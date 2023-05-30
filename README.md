# cochlear-implants

The Volume_Renderer folder and Xcode project file contains the source code for the X-ray and CT visualisation software that performs the generation of DRRs through the CT data according to data provided in the X-ray file, comparison with the X-ray image and optimisation over the 8D alignment search space to align the CT data with the X-ray. It also posses functionality to save a file with the segmented positions of the implant electrodes in the X-ray image, transformed into the coordinate system of the CT data at the alignment found; the files exported are text files, the contents of which are compatible with the Stradview '.sw' file format.

The Scripts folder contains Python and Matlab scripts for converting X-ray and CT DICOM files into readable format for the Volume_Renderer software, and scripts for visualising data outputted by the Volume_Renderer software for evaluation of optimiser performance.
