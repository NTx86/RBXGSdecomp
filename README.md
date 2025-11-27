# RBXGS Decompilation
A decompilation of [RBXGS (Roblox Grid Service) version 0.3.634.0](https://archive.org/download/rbxgssetup/S3FileHandler_RBXGSSetup_0.3.634.0.msi), currently focusing on specific critical components (v8kernel, v8world, etc.) for now, and will later expand out to other classes.

Recommended to use Microsoft Visual Studio 2005 with [SP1 Update](https://web.archive.org/web/20200801000000id_/download.microsoft.com/download/6/3/c/63c69e5d-74c9-48ea-b905-30ac3831f288/VS80sp1-KB926601-X86-ENU.exe) for matching. (Visual C/C++(14.00.50727)[C++])

You can create the [objdiff](https://github.com/encounter/objdiff) project by first exporting target objects with the [Object file exporter extension for Ghidra](https://github.com/boricj/ghidra-delinker-extension) into a `RBXGS` directory in the root of this repository, then running `configure.py` to create the project based on what you have exported.

# Dependencies
* [boost 1.34.1](https://www.boost.org/users/history/version_1_34_1.html)
* [SDL 1.2.6](https://www.libsdl.org/release/SDL-1.2.6.zip)
* [Graphics3D 6.09](https://sourceforge.net/projects/g3d/files/g3d-cpp/6.09/)
