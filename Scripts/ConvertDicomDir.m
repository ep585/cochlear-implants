function C1inverse = ConvertDicomDir(directory, outFileName, leftPresent, rightPresent, datasetName)
    fileStructs = dir(directory);
    fileNames = {fileStructs.name};
    fileNames = fileNames(3:end);

    nFiles = length(fileNames);

    cd = char(directory);
    if cd(end) ~= '/'
        directory = directory + "/";
    end
    
    % reading first two files
    infoFirst = dicominfo(directory + fileNames(1));
    first = dicomread(infoFirst);
    sz = size(first);
    assert(infoFirst.Rows == sz(1));
    assert(infoFirst.Columns == sz(2));
    pixelSpacing = infoFirst.PixelSpacing;
    xyOffset = infoFirst.ImagePositionPatient(1:2);

    infoSecond = dicominfo(directory + fileNames(2));
    second = dicomread(infoSecond);
    assert(all(size(second) == sz));
    assert(all(infoSecond.PixelSpacing == pixelSpacing));
    assert(all(infoSecond.ImagePositionPatient(1:2) == xyOffset));

    sliceSpacing = infoSecond.ImagePositionPatient(3) - infoFirst.ImagePositionPatient(3);
    orderReversed = (sliceSpacing < 0.0);
    if orderReversed
        sliceSpacing = -sliceSpacing;
    end

    X0 = infoFirst.ImageOrientationPatient(1:3);
    Y0 = infoFirst.ImageOrientationPatient(4:6);
    spaceDimensions = [flip(sz)'.*flip(pixelSpacing); nFiles*sliceSpacing];
    dataOriginOffset = infoFirst.ImagePositionPatient + 0.5*spaceDimensions;
    
    % trying to make it compatible with reverse-ordered CT files
    if orderReversed
        dataOriginOffset(3) = dataOriginOffset(3) - spaceDimensions(3);
    end

    C1 = [
        X0 Y0 [0; 0; 1.0] dataOriginOffset;
        0.0 0.0 0.0 1.0
    ];
    C1inverse = inv(C1);
    
    % presences of left and right implants:
    if leftPresent; leftByte = 1; else; leftByte = 0; end
    if rightPresent; rightByte = 1; else; rightByte = 0; end
    
    MAX_DATASET_NAME_LENGTH = 10;
    charArray = convertStringsToChars(datasetName);
    fullLengthArray = zeros(MAX_DATASET_NAME_LENGTH, 1, 'int8');
    fullLengthArray(1:length(charArray)) = charArray;

    outFile = fopen(outFileName, "w");
    % `84 + MAX_DATASET_NAME_LENGTH` byte header:
    % 2; uint16: width
    % 2; uint16: height
    % 2; uint16: depth
    % 4; float32: pixel x spacing
    % 4; float32: pixel y spacing
    % 4; float32: pixel z spacing
    % 64; 4x4 float32 column-major matrix: transformation matrix C1 inverse
    % 1; bool: left implant present
    % 1; bool: right implant present
    % `MAX_DATASET_NAME_LENGTH`; char array: dataset name
    fwrite(outFile, flip(sz), "uint16");
    fwrite(outFile, nFiles, "uint16");
    fwrite(outFile, flip(pixelSpacing), "float32");
    fwrite(outFile, sliceSpacing, "float32");
    fwrite(outFile, C1inverse, "float32"); % Matlab stores matrices column-major so no need to transpose here
    fwrite(outFile, leftByte, "uint8");
    fwrite(outFile, rightByte, "uint8");
    fwrite(outFile, fullLengthArray, "int8");
    
    if orderReversed
        for fileName = flip(fileNames(1:nFiles))
            info = dicominfo(directory + fileName);
            data = dicomread(info);
            assert(all(size(data) == sz));
            assert(all(info.PixelSpacing == pixelSpacing));
            assert(all(info.ImagePositionPatient(1:2) == xyOffset));
            assert(all(info.ImageOrientationPatient(1:3) == X0));
            assert(all(info.ImageOrientationPatient(4:6) == Y0));
            fwrite(outFile, data, "int16");
        end
    else
        fwrite(outFile, first, "int16");
        fwrite(outFile, second, "int16");
        
        for fileName = fileNames(3:nFiles)
            info = dicominfo(directory + fileName);
            data = dicomread(info);
            assert(all(size(data) == sz));
            assert(all(info.PixelSpacing == pixelSpacing));
            assert(all(info.ImagePositionPatient(1:2) == xyOffset));
            assert(all(info.ImageOrientationPatient(1:3) == X0));
            assert(all(info.ImageOrientationPatient(4:6) == Y0));
            fwrite(outFile, data, "int16");
        end
    end

    fclose(outFile);
end