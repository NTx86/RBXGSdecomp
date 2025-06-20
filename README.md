# RBXGS Decompilation
A decompilation of [RBXGS (Roblox Grid Service) version 0.3.634.0](https://archive.org/download/rbxgssetup/S3FileHandler_RBXGSSetup_0.3.634.0.msi), currently focusing on specific critical components (v8kernel, v8world, etc.) for now, and will later expand out to other classes.

Recommended to use Microsoft Visual Studio 2005 for matching. (Visual C/C++(14.00.50727)[C++])

Some tools we recommend for matching are [objdiff](https://github.com/encounter/objdiff) and the [Object file exporter extension for Ghidra](https://github.com/boricj/ghidra-delinker-extension).

# Dependencies
* [boost 1.34.1](https://www.boost.org/users/history/version_1_34_1.html)
* [SDL 1.2.6](https://www.libsdl.org/release/SDL-1.2.6.zip)
* [Graphics3D 6.09](https://sourceforge.net/projects/g3d/files/g3d-cpp/6.09/)

# Cloning using git
To clone the repo:
```
git clone https://github.com/NTx86/RBXGSdecomp.git
cd RBXGSdecomp
```
If you need to change branches:
```
git checkout BRANCH_NAME_HERE
```
To install submodules (required for building):
```
git submodule init 
git submodule update
```
