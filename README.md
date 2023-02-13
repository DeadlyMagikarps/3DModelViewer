# 3DModelViewer
Testing 3D Model Viewer using OpenGL, Assimp, and GLFW

## VCPKG Installation (Windows)
- https://vcpkg.io/en/getting-started.html

Follow step 1 and 2 to install vcpkg. Then, run the command below for assimp.

## Assimp Installation using VCGPKG
- In windows powershell, use the command ```./vcpkg.exe install assimp:x64-windows``` wherever the local vcpkg exe is located to install system wide.
This can be removed by simply replacing install with remove keyword.

After installation, run ```./vcpkg.exe integrate install```.
