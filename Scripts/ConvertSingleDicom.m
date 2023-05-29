function ConvertSingleDicom(inFileName, outFileName)
    dcmInfo = dicominfo(inFileName);
    data = dicomread(dcmInfo); % get an error here, don't know how to fix it. dicomread function just doesn't seem to work for this dcm file.
    sz = size(data);
    pixelSpacing = dcmInfo.PixelSpacing;
    
    outFile = fopen(outFileName, "w");

    % 2 + 2 + 4 + 4 = 12 byte header
    fwrite(outFile, sz(1), "uint16");
    fwrite(outFile, sz(2), "uint16");
    fwrite(outFile, pixelSpacing(1), "float32");
    fwrite(outFile, pixelSpacing(2), "float32");

    fwrite(outFile, data, "int16");

    fclose(outFile);
end