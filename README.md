#SDL Base Project

This is my SDL app skeleton.
[premake4](http://industriousone.com/premake) is used to generate the project files.

Update 2 Feb 2016:
- Implemented body of SceneManager.

Update 18 Jan 2016:
- Refactored most components
- Removed dependency on GLEW, using [glad](https://github.com/Dav1dde/glad) instead

Update 30 June 2015:
- Added handleEvents function to the Scene class.

Update 14 May 2015:
- Made Surface properly support grayscale textures

Update 4 March 2015:
- Updated Random wrapper class with a globally available instance
- Added an Elapsed class for high resolution timing.

Update 8 Jan 2015:
- Added basic GUI support using [ImGui](https://github.com/ocornut/imgui)

Update 26 Dec 2014:
- Added BasicMesh utility class
- Changed startup scene to a rotating 3D cube
- Updated the shader to render the cube

Update 24 Dec 2014:
- Added Visual Studio 12 project files
- Fixed shader compilation checking

Update 10 Dec 2014:
- Added Texture and Surface utility classes
- Make program quit when ESC key is pressed

Update 10 Sep 2014:
- Added GLEW support
- Added Shader helper class
- Added example code to render a colored quad using Vertex Array Objects instead of `glEnableClientState`
- Use glm for matrices
