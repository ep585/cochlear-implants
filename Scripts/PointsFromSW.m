function m_points = PointsFromSW(filename, pixelSpacing)
    file = fopen(filename, 'r');
    data = fread(file, 'int8=>char');
    fclose(file);

    % find x scale tag
    %i = 1;
    %while ~all(data(i:(i + 9))' == 'RES_XSCALE')
    %    while data(i) ~= 10
    %        i = i + 1;
    %    end
    %    i = i + 1;
    %end
    % skip to value
    %i = i + 11;
    %% find end of number
    %j = i;
    %while data(j) ~= 10
    %    j = j + 1;
    %end
    %xScale = str2double(data(i:(j - 1)));
    
    % find y scale tag
    %i = 1;
    %while ~all(data(i:(i + 9))' == 'RES_YSCALE')
    %    while data(i) ~= 10
    %        i = i + 1;
    %    end
    %    i = i + 1;
    %end
    %% skip to value
    %i = i + 11;
    %% find end of number
    %j = i;
    %while data(j) ~= 10
    %    j = j + 1;
    %end
    %yScale = str2double(data(i:(j - 1)));

    % find points tag
    i = 1;
    while ~all(data(i:(i + 3))' == 'CONT')
        while data(i) ~= 10
            i = i + 1;
        end
        i = i + 1;
    end
    % skip tag and first three values
    i = i + 10;
    assert(data(i) == ' ');
    for n = int8(1:44)
        i = i + 1;
        j = i;
        while data(j) ~= ' ' && data(j) ~= 10
            j = j + 1;
        end
        m_points(mod(n - 1, 2) + 1, idivide(n - 1, 2) + 1) = str2double(data(i:(j - 1))');
        i = j;
    end
    
    % reading second set of points, only if file doesn't end here
    if i + 3 < length(data)
        % find nextpoints tag
        while ~all(data(i:(i + 3))' == 'CONT')
            while data(i) ~= 10
                i = i + 1;
            end
            i = i + 1;
        end
        % skip tag and first three values
        i = i + 10;
        assert(data(i) == ' ');
        for n = int8(1:44)
            i = i + 1;
            j = i;
            while data(j) ~= ' ' && data(j) ~= 10
                j = j + 1;
            end
            m_points(mod(n - 1, 2) + 1, 22 + idivide(n - 1, 2) + 1) = str2double(data(i:(j - 1))');
            i = j;
        end
    end
    
    % apply scaling
    %m_points(1, :) = 10.0*xScale*m_points(1, :);
    %m_points(2, :) = 10.0*yScale*m_points(2, :);
    m_points = pixelSpacing.*m_points;
end