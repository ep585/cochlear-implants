% Function to find approximate transformation to match 22 implant points
% identified in 3d ct data in a .ply file and 2d xray data in a .sw file.

function FindApproxTransformation(dataName, plot)

    dataDirectory = "../Data/";
    %dataName = "First";
    %dataName = "E1N11";
    %dataName = "N18";
    outputDirectory = "/Users/eprager/Projects/General/Volume_Renderer/Volume_Renderer/Inputs/";
    
    % reading the X-ray width and height from the X-ray file's metadata
    dcmInfo = dicominfo(dataDirectory + dataName + "/x_ray/x_ray.dcm");
    usingScaledSpacing = isfield(dcmInfo, "PixelSpacing");
    if ~usingScaledSpacing && ~isfield(dcmInfo, "ImagerPixelSpacing")
        throw(MException("FindApproxTransformation:ReadXRayDICOM", "No pixel spacing field."));
    end
    if usingScaledSpacing
        pixelSpacing = flip(dcmInfo.PixelSpacing);
        sourceZ = -dcmInfo.DistanceSourceToPatient;
    else
        pixelSpacing = flip(dcmInfo.ImagerPixelSpacing);
        sourceZ = -dcmInfo.DistanceSourceToDetector;
    end
    xrayOriginOffset = [-0.5*cast(dcmInfo.Columns, "double"); -0.5*cast(dcmInfo.Rows, "double")] .* pixelSpacing;
    
    % reading X-ray electrode positions from Stradview file
    points2d = PointsFromSW(dataDirectory + dataName + "/x_ray.sw", pixelSpacing);
    
    % transforming X-ray eletrode locations to the appropriate coordinate
    % system (origin in centre of X-ray, (x, y) measured in mm, positive up and to
    % right):
    points2d = points2d + xrayOriginOffset;
    points2d(2, :) = -points2d(2, :);
    WritePoints(points2d, outputDirectory + "points2d.dat");
    
    points3d = [];
    p3dN = 0;
    % reading left 3d points if they exist
    name = dataDirectory + dataName + "/left scala tympani.ply";
    leftPresent = isfile(name);
    if leftPresent
        points3d = [points3d PointsFromPLY(name)];
        assert(all(size(points3d) == [3 p3dN + 22]));
        p3dN = p3dN + 22;
    end
    % reading right 3d points if they exist
    name = dataDirectory + dataName + "/right scala tympani.ply";
    rightPresent = isfile(name);
    if rightPresent
        points3d = [points3d PointsFromPLY(name)];
        assert(all(size(points3d) == [3 p3dN + 22]));
        p3dN = p3dN + 22;
    end
    assert(width(points2d) == p3dN);

    % transforming CT (approx.) electrode locations to the appropriate
    % coordinate system (origin in centre of CT space, (x, y, z) measured in mm
    % with directions consistent with the CT data:
    C1inverse = ConvertDicomDir(dataDirectory + dataName + "/ct", outputDirectory + "ct.dat", leftPresent, rightPresent, dataName);
    points3dHomogeneous = [points3d; ones(1, p3dN)];
    points3dHomogeneous = C1inverse*points3dHomogeneous;
    points3d = points3dHomogeneous(1:3, :)./points3dHomogeneous(4, :);
    WritePoints(points3d, outputDirectory + "points3d.dat");
    
    % copying the otic capsule meshes if they exist
    name = dataDirectory + dataName + "/left otic capsule.ply";
    if isfile(name)
        copyfile(name, outputDirectory);
    end
    name = dataDirectory + dataName + "/right otic capsule.ply";
    if isfile(name)
        copyfile(name, outputDirectory);
    end

    % doesn't work for some reason - Matlab's dicom reader is dodgy
    %ConvertSingleDicom(inputDirectory + "X-ray.dcm", outputDirectory + "xray.dat"); 
    % using python instead for this
    
    options = optimoptions('lsqcurvefit','Algorithm','levenberg-marquardt');
    lb = [-50, -50, -500, -2*pi, -pi/2, -2*pi, -20, -20];
    ub = [ 50,  50,  500,  2*pi,  pi/2,  2*pi,  20,  20];
    objectiveFunction = @(parameters, v3)Project3dTo2d(parameters, v3, sourceZ);
    
    n_repeats = 500;
    xs = zeros(n_repeats, 8);
    resnorms = zeros(1, n_repeats);
    for i = 1:n_repeats
        [xs(i, :), resnorms(i)] = lsqcurvefit(objectiveFunction, [0.0 0.0 0.0 -pi+2.0*pi*rand() -pi+2.0*pi*rand() 2.0*pi*rand() 0.0 0.0], points3d, points2d, lb, ub);
    end
    [resnorm, xi] = min(resnorms);
    x = xs(xi, :);
    
    x(4:6) = mod(x(4:6) + [pi pi pi], 2.0*pi) - [pi pi pi];
    
    % saving result
    outFile = fopen(outputDirectory + "initialAlignment.dat", "w");
    % 32 byte data file:
    fwrite(outFile, x, "float32");
    fclose(outFile);
    
    
    % displaying result
    disp("Target:");
    disp(points2d');
    disp("Reached:")
    reached = Project3dTo2d(x, points3d, sourceZ);
    disp(reached');
    
    disp("Residual norm:");
    disp(resnorm);
    
    disp("{{" + x(1) + "f, " + x(2) + "f, " + x(3) + "f}, {" + x(4) + "f, " + x(5) + "f, " + x(6) + "f}, {" + x(7) + ", " + x(8) + "}};");
    
    if plot
        scatter(points2d(1, :), points2d(2, :), "DisplayName", "X-ray electrode positions"); hold on;
        scatter(reached(1, :), reached(2, :), "DisplayName", "Transformed approx. CT electrode positions"); hold off;
        legend();
    end
end
