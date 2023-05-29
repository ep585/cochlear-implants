dcmInfo = dicominfo("DICOM_Horos_Exports/xray/IM-0001-0001.dcm");

psPhys = dcmInfo.ImagerPixelSpacing;
ps = dcmInfo.PixelSpacing;
fovDimsMM = cast(dcmInfo.FieldOfViewDimensions, "double");
fovOriginPixelsPhys = cast(dcmInfo.FieldOfViewOrigin, "double");
activeDimsMM = dcmInfo.DetectorActiveDimensions;

fovOriginMM = psPhys.*fovOriginPixelsPhys;

srcPos = 0.5*fovDimsMM + fovOriginMM - 0.5*activeDimsMM;

disp(srcPos);