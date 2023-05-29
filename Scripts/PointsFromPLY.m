function m_points = PointsFromPLY(filename)
    file = fopen(filename, 'r');
    data = fread(file, 'int8=>char');
    fclose(file);
    
    m_points = zeros(3, 22);

    % skip two lines
    i = 1;
    for n = 1:2
        while data(i) ~= 10
            i = i + 1;
        end
        i = i + 1;
    end
    
    % read 22 comments
    for n = 1:22
        % should be a comment
        assert(all(data(i:(i + 6))' == "comment"));
        i = i + 7;
        % read until '"' twice
        for m = 1:2
            while data(i) ~= '"'
                i = i + 1;
            end
            i = i + 1;
        end
        assert(data(i) == ' ');
        % read three numbers
        for m = 1:3
            i = i + 1;
            j = i;
            while data(j) ~= ' '
                j = j + 1;
            end
            m_points(m, n) = str2double(data(i:(j - 1)));
            i = j;
        end
        % read to next line
        while data(i) ~= 10
            i = i + 1;
        end
        i = i + 1;
    end
end