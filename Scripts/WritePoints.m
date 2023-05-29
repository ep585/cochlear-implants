function WritePoints(matrixPoints, outFileName)
    [d, n] = size(matrixPoints);
    outFile = fopen(outFileName, "w");

    % 8 byte header:
    % 4; uint32: dimensions
    % 4; uint32: number
    fwrite(outFile, d, "uint32");
    fwrite(outFile, n, "uint32");
    
    % values stored as 32-bit floats
    fwrite(outFile, matrixPoints, "float32");

    fclose(outFile);
end