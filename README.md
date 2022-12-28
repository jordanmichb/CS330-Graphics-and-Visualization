# CS330-Graphics-and-Visualization
<b> About the Project </b>

This project utilizes the OpenGL library and C++ to reproduce a scene from a picture by constructing 3D objects. The scene is a ball, a notepad, a pencil on top of the notepad, and a box. All of these items are placed on a plane. Each item and the plane have a texture that was placed by mapping texture coordinates and each object reflects light from the three light sources by the mapping of surface normals. The user's view is not fixed, meaning that the camera can be moved around the scene.



<b> Project Functionality </b>

The scene begins at coordinate (0, 0, 3). The camera can be moved up, left, down, or right by using the W, A, S, D keys respectively, and can be zoomed in or out by using the Q and E keys. The speed of camera movement can be changed by using the mouse scroll button; scroll up to increase the speed and scroll down to decrease it. Lastly, the view can be changed from a perspective projection (3D) into an orthographic projection (2D) by pressing the P key.

Scene: 

![image](https://user-images.githubusercontent.com/95947696/209863097-8cd28a80-e8a0-401f-b8d2-f9805665f438.png = 400x300)

<br>
<br>

<b> Tools Used </b>

MongoDB
<br>
&emsp;•	MongoDB was used because it is a good way to store and retrieve data and is compatible with many programming languages, including Python which was used for this project. It stores data in BSON format meaning that applications can receive the data in JSON format, which is helpful when using MongoDB with Python because using Python dictionaries make both simple and advanced querying a straightforward task. MongoDB has a driver for Python, PyMongo, which allows for easy connection to the database.
<br>
<br>
Dash Framework
<br>
&emsp;•	Dash was used to build the dashboard and is great for building a customized interface, especially with Python. It provides methods to build many different interactive options, such as the radio items, datatable, geolocation map, and pie chart that were used in this project. Combined with Dash callbacks, a fully interactive and functional dashboard interface can be built. Since it is compatible with Python and Python is compatible with MongoDB, it is a valuable tool for integrating the functionalities of all three together.
<br>
<br>
Jupyter Notebook
<br>
&emsp;•	Jupyter Notebook was used to write the CRUD Python module as a .py file, and to test the module in a .ipynb Python script. The dashboard was also created in a .ipynb Python script.

<br>
<br>

<b> Reproducing the Project </b>

•	Import the AAC database through the terminal, contained in aac_shelter_outcomes.csv.
<br>
•	Enable user authentication for the database and connect to the MongoDB server under that user.
<br>
•	Use a tool like Jupyter Notebook to open the .ipynb dashboard file and the .py CRUD Python module.
<br>
•	In the dashboard code, edit the instantiation of the CRUD Python module to the correct username, password, and port number.
<br>
•	Run the code to view the dashboard. As mentioned before the pie chart will be crowded, so double-click on one breed to isolate and single-click to add more breeds to the chart.
<br>
•	Use the radio buttons next to each row to see that animal’s location on the map. Hover over the marker to see their breed and click the marker to see their name.
<br>
•	Use the radio buttons above the chart to filter for dogs that are best suited to the job specified by the button. Again, breeds on the pie chart may be selected/deselected for a customized view.
<br>
•	If more specific filters are desired, text can be typed in the empty box above any column to perform a specific query.
<br>
•	Click the Grazioso Salvare logo to be redirected to the client homepage.
