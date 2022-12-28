#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <math.h>
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dependencies/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"

#include "Mesh.h"

using namespace std;

/* Shader program Macro */
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace
{
	// Window dimensions
	const GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
	// Main window
	GLFWwindow* window = nullptr;

	// Shader program ID
	GLuint objectShaderId;
	GLuint lightShaderId;

	// Texture IDs
	GLuint textureIdPencil;
	GLuint textureIdTip;
	GLuint textureIdPlane;
	GLuint textureIdPaper;
	GLuint textureIdBox;
	GLuint textureIdBall;

	// For camera control
	Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// To handle zooming in orthographic view
	bool perspective = true;
	GLfloat orthoCoords[4] = { 0.0f, 5.0f, 0.0f, 4.0f }; // Left, right, bottom, top

	// For timing
	float gDeltaTime = 0.0f;  // Time between current frame and previous frame
	float gLastFrame = 0.0f;
}

/**************************************************************************
*																		  *
*						 	 OBJECT SHADERS                               *
*                                                                         *
**************************************************************************/

/* Object Vertex Shader */
//////////////////////////
const char* objectVertexShader = GLSL(330,
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec2 textureCoordinate;

	out vec3 vertexNormal;
	out vec3 vertexFragmentPos;
	out vec2 vertexTextureCoordinate;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f);
		vertexFragmentPos = vec3(model * vec4(position, 1.0f));
		vertexNormal = mat3(transpose(inverse(model))) * normal;
		vertexTextureCoordinate = textureCoordinate;
	}
);

/* Object Fragmet Shader */
///////////////////////////
const char* objectFragmentShader = GLSL(330,
	in vec3 vertexNormal;
	in vec3 vertexFragmentPos;
	in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor;

	uniform vec3 objectColor;
	uniform vec3 viewPosition;
	uniform sampler2D uTexture;
	uniform bool hasTexture;

	struct Light {
		vec3 position; // Light position
		vec3 color; // Light color
		vec3 direction;

		float intensity; // Intensity percentage ranging from 0.0 to 1.0
	};

	const int NR_LIGHTS = 3;
	uniform Light lights[NR_LIGHTS];

	vec3 CalcPhong(Light light);

	void main()
	{
		vec3 result;

		for (int i = 0; i < NR_LIGHTS; i++) // Loop over the lights 
		{
			result += CalcPhong(lights[i]);
		}
		fragmentColor = vec4(result, 1.0); // Send lighting results to GPU
	}

	vec3 CalcPhong(Light light)
	{
		float attenuation = 1.0f;
		vec3 lightDirection = normalize(-light.direction);

		// If there is no direction vector, light is a point light so recalculate attenuation and light direction
		if (light.direction == vec3(0.0))
		{
			// Calculate attenuation
			float distance = length(light.position - vertexFragmentPos);
			attenuation = light.intensity / (1.0f + 0.09f * distance + 0.032f * (distance * distance));
			lightDirection = normalize(light.position - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
		}

		// Calculate Ambient lighting
		float ambientStrength = 0.1f; // Set ambient or global lighting strength
		vec3 ambient = ambientStrength * light.color * attenuation; // Generate ambient light color

		// Calculate Diffuse lighting
		vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
		float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
		vec3 diffuse = impact * light.color * attenuation; // Generate diffuse light color

		// Calculate Specular lighting
		float specularIntensity = 0.8f; // Set specular light strength
		float highlightSize = 16.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
		// Calculate specular component
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		vec3 specular = specularIntensity * specularComponent * light.color * attenuation;

		if (hasTexture)
		{
			// Texture holds the color to be used for all three components
			vec4 textureColor = texture(uTexture, vertexTextureCoordinate);
			// Calculate phong result
			return (ambient + diffuse + specular) * textureColor.xyz;
		}
		else
		{
			return (ambient + diffuse + specular) * objectColor;
		}
	}
);

/**************************************************************************
*																		  *
*				          LIGHT SOURCE SHADERS                            *
*                                                                         *
**************************************************************************/

/* Light Vertex Shader */
/////////////////////////
const GLchar* lightVertexShader = GLSL(440,
	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

	//Uniform / Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
	}
);

/* Light Fragment Shader*/
//////////////////////////
const GLchar* lightFragmentShader = GLSL(440,
	out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

	void main()
	{
		fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
	}
);

/**************************************************************************
*																		  *
*					      FORWARD DECLARATIONS                            *
*                                                                         *
**************************************************************************/

bool Initialize();
bool CreateTexture(const char* filename, GLuint& textureId);
bool LoadTextures();
bool CreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
// void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void DestroyShaderProgram(GLuint programId);

int main()
{
	// Set up window
	if (!Initialize())
	{
		return EXIT_FAILURE;
	}

	/*
	 * Create objects
	 */
	Mesh plane, pencilBody, pencilTip, notepad, box, sphere;
	plane.CreatePlane();
	pencilBody.CreateCylinder(0.1f, 0.1f, 10.0f, 4.5f, 4.0f); // Params: base radius, top radius, sectors, height, stacks
	pencilTip.CreateCylinder(0.1f, 0.003f, 10.0f, 0.5f, 4.0f);
	notepad.CreateCube(2.5f, 0.3f, 2.5f); // Params: length, height, width
	box.CreateCube(3.0f, 1.0f, 2.25f);
	sphere.CreateSphere(0.5f, 36, 18); // Params: radius, sectors, stacks

	/*
	 * Create and compile shaders
	 */
	CreateShaderProgram(objectVertexShader, objectFragmentShader, objectShaderId);
	CreateShaderProgram(lightVertexShader, lightFragmentShader, lightShaderId);

	/*
	 * Load textures
	 */
	if (!LoadTextures())
	{
		return EXIT_FAILURE;
	}

	// Set shader
	glUseProgram(objectShaderId);
	glUniform1i(glGetUniformLocation(objectShaderId, "uTexture"), 0); // Texture unit 0

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable z-depth
	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Timing by frame
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// For processing input
		ProcessInput(window);

		// Render objects
		plane.RenderPlane(objectShaderId, textureIdPlane, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);
		pencilBody.RenderPencilBody(objectShaderId, textureIdPencil, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);
		pencilTip.RenderPencilTip(objectShaderId, textureIdTip, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);
		notepad.RenderNotepad(objectShaderId, textureIdPaper, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);
		box.RenderBox(objectShaderId, textureIdBox, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);
		// Draws sphere AND lights
		sphere.RenderSphere(objectShaderId, lightShaderId, textureIdBall, WINDOW_WIDTH, WINDOW_HEIGHT, gCamera, perspective, orthoCoords);

		// Get and handle user input events
		glfwPollEvents();

		// Swap back buffer and front buffer each frame
		glfwSwapBuffers(window);
	}
	// Release shader programs
	DestroyShaderProgram(objectShaderId);
	DestroyShaderProgram(lightShaderId);


	exit(EXIT_SUCCESS);
}

/* Initialize GlfW and Glew, and create a window */
///////////////////////////////////////////////////
bool Initialize()
{
	// Set up GLFW window properties
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// No backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Final Project", NULL, NULL);

	if (window == NULL) // If window creation fails
	{
		cout << "Failed to create GLFW window." << endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Register callback functions for handling mouse events
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	// glfwSetMouseButtonCallback(window, MouseButtonCallback);

	// Tell GLFW to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Allow modern features
	GLenum GlewInitResult = glewInit();

	// If GLEW initialization fails
	if (GlewInitResult != GLEW_OK)
	{
		cerr << glewGetErrorString(GlewInitResult) << endl;
		return false;
	}

	return true;
}

/* Generate and load a texture */
/////////////////////////////////
bool CreateTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;

	// Flip image so it appears right side up
	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Set wrapping parameters for both x and y axes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Generate texture
		if (channels == 3) // RGB
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		else if (channels == 4) // RGBA
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else
		{
			cout << "Cannot load image with " << channels << " channels" << endl;
			return false;
		}

		// Release the image
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	return false; // Error
}

/* Load in texture files */
///////////////////////////
bool LoadTextures()
{
	const char* texFilename = "textures/pencil.jpg";
	if (!CreateTexture(texFilename, textureIdPencil))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	texFilename = "textures/penciltip.jpg";
	if (!CreateTexture(texFilename, textureIdTip))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	texFilename = "textures/table.jpg";
	if (!CreateTexture(texFilename, textureIdPlane))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	texFilename = "textures/notepad.jpg";
	if (!CreateTexture(texFilename, textureIdPaper))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	texFilename = "textures/box.jpg";
	if (!CreateTexture(texFilename, textureIdBox))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	texFilename = "textures/ball.jpg";
	if (!CreateTexture(texFilename, textureIdBall))
	{
		cout << "Failed to load texture: " << texFilename << endl;
		return false;
	}

	return true;
}

/* Compile shaders and check for errors */
//////////////////////////////////////////
bool CreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// For error reporting
	int success = 0;
	char infoLog[512];

	programId = glCreateProgram();

	// Create vertex shader and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile vertex shader
	glCompileShader(vertexShaderId);
	// Check for errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, sizeof(infoLog), NULL, infoLog);
		cout << "Vertex shader compilation failed." << infoLog << endl;
		return false;
	}

	// Compile fragment shader
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		cout << "Fragment shader compilation failed." << infoLog << endl;
		return false;
	}

	// Attach both shaders to the program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);
	// Check for errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		cout << "Error linking shader program." << infoLog << endl;
		return false;
	}

	// Use shader program
	glUseProgram(programId);

	return true;
}

/* Check if escape key is pressed, and if so set that window should close */
////////////////////////////////////////////////////////////////////////////
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraOffset = gCamera.MovementSpeed * gDeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		// Move forward
		if (!perspective) // If in orthographic view adjust frustum to be narrower
		{
			orthoCoords[0] += cameraOffset;
			orthoCoords[1] -= cameraOffset;
			orthoCoords[2] += cameraOffset;
			orthoCoords[3] -= cameraOffset;
		}
		else
		{
			gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		// Move backwards
		if (!perspective) // If in orthographic view adjust frustum to be wider
		{
			orthoCoords[0] -= cameraOffset;
			orthoCoords[1] += cameraOffset;
			orthoCoords[2] -= cameraOffset;
			orthoCoords[3] += cameraOffset;
		}
		else
		{
			gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		// Move left
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		// Move right
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		// Move up
		gCamera.Position += cameraOffset * gCamera.Up;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		// Move down
		gCamera.Position -= cameraOffset * gCamera.Up;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		// Toggle perspective and orthographic view
		glfwWaitEventsTimeout(0.7); // Prevent mutltiple toggles with one key press 
		perspective = !perspective;
	}
}

/* Executes when window size changes */
///////////////////////////////////////
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* Called whenever mouses moves */
//////////////////////////////////
void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Prevent camrea jump when mouse first enters window
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}
	// Movement between last and current frame
	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos;

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

/* Called when scroll wheel moves */
//////////////////////////////////// 
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Adjust movement speed by scrolling
	gCamera.MovementSpeed += yoffset;

	if (gCamera.MovementSpeed < 0.5f)
		gCamera.MovementSpeed = 0.5f;
	if (gCamera.MovementSpeed > 30.5f)
		gCamera.MovementSpeed = 30.5f;
}

/* Release shader */
////////////////////
void DestroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}