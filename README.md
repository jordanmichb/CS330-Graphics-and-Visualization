# CS330-Graphics-and-Visualization
<b> About the Project </b>

This project utilizes the OpenGL library and C++ to reproduce a scene from a picture by constructing 3D objects. The scene is a ball, a notepad, a pencil on top of the notepad, and a box. All of these items are placed on a plane. Each item and the plane have a texture that was placed by mapping texture coordinates and each object reflects light from the three light sources by the mapping of surface normals. The user's view is not fixed, meaning that the camera can be moved around the scene.



<b> Project Functionality </b>

The scene begins at coordinate (0, 0, 3). The camera can be moved up, left, down, or right by using the W, A, S, D keys respectively, and can be zoomed in or out by using the Q and E keys. The speed of camera movement can be changed by using the mouse scroll button; scroll up to increase the speed and scroll down to decrease it. Lastly, the view can be changed from a perspective projection (3D) into an orthographic projection (2D) by pressing the P key.

Scene: 

![image](https://user-images.githubusercontent.com/95947696/209863681-ebe0e9a9-a30a-44dd-b235-5e6c119bef45.png)

<br>
<br>

<b> Tools Used </b>

OpenGL
<br>
&emsp;•	This project aimed to render 2D and 3D vector graphics. OpenGL is an API that is able to interact with the GPU and provide the desired functionality.

<br>
<br>

<b> Reproducing the Project </b>

•	Download the provided files.
<br>
•	Place the OpenGL folder into FinalProject folder's first subdirectory, in the same location as the solution file.
<br>
•	Import the project into a software that can run C++ code, such as Visual Studio an run the main.cpp file.
<br>
•	In Visual Studio, if the code will not build, switch to the x86 configuration.
<br>
•	Use the W, A, S, D keys for movement along the xy-plane and the Q and E keys to move along the z-plane. Use the mouse scroll to control movement speed and the P key to switch between orthographic and perspectiive views.
<br>
•	The program will capture the mouse. To terminate the program, press the escape key.
