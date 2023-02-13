# 3DModelViewer
Testing 3D Model Viewer using OpenGL, Assimp, and GLFW

## VCPKG Installation (Windows)
- https://vcpkg.io/en/getting-started.html

Follow step 1 and 2 to install vcpkg. Then, run the command below for assimp.

## Assimp Installation using VCGPKG
- In windows powershell, use the command ```./vcpkg.exe install assimp:x64-windows``` wherever the local vcpkg exe is located to install system wide.
This can be removed by simply replacing install with remove keyword.

After installation, run ```./vcpkg.exe integrate install```.

## Running Program
To run the debugger in Visual Studio, make sure to set it to x64. It is not build with x86 for simplicity.
Both Debug and Release should work and you can just click Local Windows Debugger. If everything is installed corerctly,
it should run in both environments.

## Notes
There are a lot elements in this sample program that are not optimized or structured the way that would best showcase my
coding style. Because of this timeline and many personal responsibilities, I took the path of least resistance to share
concepts rather than making it look pretty. I have had a lot of hats during the past few years. 

I have not had a chance to debug the fragment and vertex shaders to display texture correctly. So you will see a black model for now.

The UI uses IMGUI. Which is mainly best for debugging and quick prototyping. Many things that I wish I had time and more knowledge to implement
was native buttons and a filesystem to retrieve from anywhere. I'm less familiar with Windows System environemnts compared to a traditional
linux/MacOS environment.

## Previous Experience
My previous experience was focused on software programs that already exist on market. I never had to build the GUI from scratch like this in over 5 years. 
It was a great learning experience overall. I have not spent time in the behind the scenes graphics applications and focused on the approach of models ready to go.

I will present a photogrammetry pipeline that was modified slightly and based on VCGLib. Other technologies used was Structure from Motion (SfM) and Multi-View Stereo (MVS) reconstruction. 
I was also using Poisson methods for reconstruction as well as Mult-View Environment (MVE) for texture reprojection. Combining python for Blender and VCGLib based on C++. This was ran on a
kubernetes cluster setup by the DevOps team.

I have less experience with the creation of the software GUI itself. If presented the opportunity to learn, grow, and share my insight in this field, I'm open for positions that would put me into a
Junior to Mid level role with the appropriate pay scale. 