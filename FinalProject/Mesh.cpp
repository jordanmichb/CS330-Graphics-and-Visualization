#include "Mesh.h"

// Light color, position, direction, scale
glm::vec3 gKeyLightColor(1.0f, 1.0f, 1.0f);
glm::vec3 gKeyLightPosition(0.0f, 10.0f, 0.0f); // Use for point lights
glm::vec3 gKeyLightDirection(-0.2f, -1.0f, -0.3f); // Use for directional lights

glm::vec3 gFillLightColor(1.0f, 0.97f, 0.61f);
glm::vec3 gFillLightPosition(7.0f, 1.0f, 0.0f); // Use for point lights

glm::vec3 gFillLightColor2(1.0f, 0.97f, 0.61f);
glm::vec3 gFillLightPosition2(0.0f, 1.0f, -7.0f); // Use for point lights

glm::vec3 gLightScale(0.3f);

/* Constructor */
/////////////////
Mesh::Mesh()
{
	vao = 0;
	vbo = 0;
	ibo = 0;
	nVertices = 0;
}

/* Create the plane as the scene's base */
//////////////////////////////////////////
void Mesh::CreatePlane()
{
	GLfloat vertices[] =
	{
		// Vertex            // Normals          // Texture
	   -5.0f,  0.0f, -4.0f,  0.0f, 70.0f, 0.0f,  0.0f, 1.0f, // Back Left
		5.0f,  0.0f, -4.0f,  0.0f, 70.0f, 0.0f,  1.0f, 1.0f, // Back Right
	   -5.0f,  0.0f,  3.0f,	 0.0f, 70.0f, 0.0f,  0.0f, 0.0f, // Front Left

	   -5.0f,  0.0f,  3.0f,  0.0f, 70.0f, 0.0f,  0.0f, 0.0f, // Front Left
		5.0f,  0.0f, -4.0f,  0.0f, 70.0f, 0.0f,  1.0f, 1.0f, // Back Right
		5.0f,  0.0f,  3.0f,  0.0f, 70.0f, 0.0f,  1.0f, 0.0f, // Front Right
	};

	// Number of coordinates per vertex (x, y, z)
	const GLuint floatsPerVertex = 3;
	// Number of normals
	const GLuint floatsPerNormal = 3;
	// Number of texture coordinates
	const GLuint floatsPerUV = 2;

	// Stride between coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);
	// Get size of vertices and indices for render params
	GLuint sizeVertices = sizeof(vertices);

	// Get number of indices
	nVertices = sizeof(vertices) / (sizeof(vertices[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	// Create VAO to store VBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create buffer object inside VAO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret vertex data
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// For normals
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	// For texture
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

/* Create a cube with a given length, height, and width */
//////////////////////////////////////////////////////////
void Mesh::CreateCube(float length, float height, float width)
{
	GLfloat vertices[] = {
		// Vertex                  // Normals            // Texture
		//--------------------------------------------------------------------------
		// Front face
		0.0f,    0.0f,    0.0f,    0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  // Bottom left
		length,  height,  0.0f,    0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  // Top right
		0.0f,    height,  0.0f,    0.0f,  0.0f,  1.0f,   0.0f,  1.0f,  // Top left

		0.0f,    0.0f,    0.0f,    0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  // Bottom left
		length,  0.0f,    0.0f,    0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  // Bottom right
		length,  height,  0.0f,    0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  // Top right
		// Left face
		0.0f,    0.0f,   -width,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  // Back bottom left
		0.0f,    height,  0.0f,   -1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Top left
		0.0f,    height, -width,  -1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  // Back top left

		0.0f,    0.0f,   -width,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  // Back bottom left
		0.0f,    0.0f,    0.0f,   -1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  // Bottom left
		0.0f,    height,  0.0f,   -1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Top Left
		// Back face
		length,  0.0f,   -width,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,  // Back bottom right
		0.0f,    height, -width,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f,  // Back top left
		length,  height, -width,   0.0f,  0.0f, -1.0f,   0.0f,  1.0f,  // Back top right

		length,  0.0f,   -width,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,  // Back bottom right
		0.0f,    0.0f,   -width,   0.0f,  0.0f, -1.0f,   1.0f,  0.0f,  // Back bottom left
		0.0f,    height, -width,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f,  // Back top left
		// Right face
		length,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  // Bottom right
		length,  height, -width,   1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Back top right
		length,  height,  0.0f,    1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  // Top right

		length,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  // Bottom right
		length,  0.0f,   -width,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  // Back bottom right
		length,  height, -width,   1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Back top right
		// Top face
		0.0f,    height,  0.0f,    0.0f,  1.0f,  0.0f,   0.0f,  0.0f,  // Top Left
		length,  height, -width,   0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  // Back top right
		0.0f,    height, -width,   0.0f,  1.0f,  0.0f,   0.0f,  1.0f,  // Back top left

		0.0f,    height,  0.0f,    0.0f,  1.0f,  0.0f,   0.0f,  0.0f,  // Top Left
		length,  height,  0.0f,    0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  // Top right
		length,  height, -width,   0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  // Back top right
		// Bottom face
		0.0f,    0.0f,   -width,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,  // Back bottom left
		length,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Bottom right
		0.0f,    0.0f,    0.0f,   -1.0f,  0.0f,  0.0f,   0.0f,  1.0f,  // Bottom left

		0.0f,    0.0f,   -width,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,  // Back bottom left
		length,  0.0f,   -width,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  // Back bottom right
		length,  0.0f,    0.0f,    1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  // Bottom right
	};
	// Number of coordinates per vertex (x, y, z)
	const GLuint floatsPerVertex = 3;
	// Number of normals
	const GLuint floatsPerNormal = 3;
	// Number of texture coordinates
	const GLuint floatsPerUV = 2;

	// Stride between coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);
	// Get size of vertices and indices for render params
	GLuint sizeVertices = sizeof(vertices);

	// Get number of indices
	nVertices = sizeof(vertices) / (sizeof(vertices[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	// Create VAO to store VBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create buffer object inside VAO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret vertex data
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// For normals
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	// For texture
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

/* Create a sphere with a given radius, number of sectors, and number of stacks */
//////////////////////////////////////////////////////////////////////////////////
void Mesh::CreateSphere(float radius, float sectorCount, float stackCount)
{
	const float PI = 3.1415926f;
	float x, y, z, xy;                              // Vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // Normal
	float s, t;                                     // Texture coordinate

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	vector<GLfloat> vertices;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // Starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// Add (sectorCount+1) vertices per stack
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // Starting from 0 to 2pi

			// Vertices
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Normals
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);

			// Texture
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			vertices.push_back(s);
			vertices.push_back(t);
		}
	}

	vector<GLuint> indices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // Beginning of current stack
		k2 = k1 + sectorCount + 1;      // Beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// Two triangles per sector excluding first and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	// Number of coordinates per vertex (x, y, z)
	const GLuint floatsPerVertex = 3;
	// Number of normals
	const GLuint floatsPerNormal = 3;
	// Number of texture coordinates
	const GLuint floatsPerUV = 2;

	GLuint sizeVertices = sizeof(vector<GLfloat>) + (sizeof(GLfloat) * vertices.size());
	GLuint sizeIndices = sizeof(vector<GLuint>) + (sizeof(GLuint) * indices.size());

	GLfloat* vertexData = vertices.data();
	GLuint* indexData = indices.data();

	nVertices = sizeIndices;
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create VAO to store VBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indexData, GL_STATIC_DRAW);

	// For vertex coordinates
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// For normal coordinates
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	// For texture coordinates
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

/* Create a cylinder with a given radius, number of sectors, height, and number of stacks */
/* Use a different radius for base and top circle to create a cone                        */
////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::CreateCylinder(float baseRadius, float topRadius, float sectorCount, float height, float stackCount)
{
	const float PI = 3.1415926f;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle;  // Angle of each sector
	vector<GLfloat> vertices;
	GLfloat x, y, z, radius;

	vector<GLfloat> unitVertices = getUnitCircleVertices(sectorStep, sectorCount);

	// Compute the normal vector at 0 degree first
	float zAngle = atan2(baseRadius - topRadius, height);
	vector<GLfloat> normals = getCylinderNormals(sectorStep, sectorCount, zAngle);

	// Get vertices of cylinder sides
	for (int i = 0; i <= stackCount; ++i)
	{
		z = -(height * 0.5f) + (float)i / stackCount * height;      // Vertex position z
		radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius); // Keep track of radius for different base sizes
		float t = 1.0f - (float)i / stackCount;   // Top to bottom

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			x = unitVertices[k];
			y = unitVertices[k + 1];
			// Vertex coordinates
			vertices.push_back(x * radius);
			vertices.push_back(y * radius);
			vertices.push_back(z);
			// Normals
			vertices.push_back(normals[k]);
			vertices.push_back(normals[k + 1]);
			vertices.push_back(normals[k + 2]);
			// Texture coordinates
			vertices.push_back((float)j / sectorCount);
			vertices.push_back(t);
		}
	}

	// Keep track of where the base top vertices start, used for indices
	unsigned int baseVertexIndex = (unsigned int)vertices.size() / 8;

	// Put vertices of base of cylinder
	z = -height * 0.5f;

	// Vertex coordinates
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);
	// Normals
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(-1);
	// Texture coordinates
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitVertices[j];
		y = unitVertices[j + 1];
		// Vertex coordinates
		vertices.push_back(x * baseRadius);
		vertices.push_back(y * baseRadius);
		vertices.push_back(z);
		// Normals
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(-1);
		// Texture coordinates
		vertices.push_back(-x * 0.5f + 0.5f);
		vertices.push_back(-y * 0.5f + 0.5f);
	}

	// Keep track of where the base base vertices start, used for indices
	unsigned int topVertexIndex = (unsigned int)vertices.size() / 8;

	// Get vertices of top of cylinder
	z = height * 0.5f;
	// Vertex coordinates
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);
	// Normals
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	// Texture coordinates
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitVertices[j];
		y = unitVertices[j + 1];
		// Vertex coordinates
		vertices.push_back(x * topRadius);
		vertices.push_back(y * topRadius);
		vertices.push_back(z);
		// Normals
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(1);
		// Texture coordinates
		vertices.push_back(x * 0.5f + 0.5f);
		vertices.push_back(-y * 0.5f + 0.5f);
	}

	// Get indices for drawing
	vector<GLuint> indices = getCylinderIndices(stackCount, sectorCount, baseVertexIndex, topVertexIndex);

	// Number of coordinates per vertex (x, y, z)
	const GLuint floatsPerVertex = 3;
	// Number of normals
	const GLuint floatsPerNormal = 3;
	// Number of texture coordinates
	const GLuint floatsPerUV = 2;

	GLuint sizeVertices = sizeof(vector<GLfloat>) + (sizeof(GLfloat) * vertices.size());
	GLuint sizeIndices = sizeof(vector<GLuint>) + (sizeof(GLuint) * indices.size());

	GLfloat* vertexData = vertices.data();
	GLuint* indexData = indices.data();

	nVertices = sizeIndices;
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create VAO to store VBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indexData, GL_STATIC_DRAW);

	// For vertex coordinates
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// For texture coordinates
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	// For texture coordinates
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

/* Get unit circle vertices for a cylinder */
/////////////////////////////////////////////
vector<GLfloat> Mesh::getUnitCircleVertices(float sectorStep, float sectorCount)
{
	float sectorAngle;
	vector<GLfloat> unitVerts;

	// Get vertices of unit circle
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitVerts.push_back(cos(sectorAngle)); // x
		unitVerts.push_back(sin(sectorAngle)); // y
		unitVerts.push_back(0);                // z
	}

	return unitVerts;
}

/* Get normals for a cylinder */
////////////////////////////////
vector<GLfloat> Mesh::getCylinderNormals(float sectorStep, float sectorCount, float zAngle)
{
	float sectorAngle;

	// Compute the normal vector at 0 degree first
	float x0 = cos(zAngle);
	float y0 = 0;
	float z0 = sin(zAngle);

	// Rotate (x0, y0, z0) per sector angle
	std::vector<GLfloat> norms;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		norms.push_back(cos(sectorAngle) * x0 - sin(sectorAngle) * y0);   // nx
		norms.push_back(sin(sectorAngle) * x0 + cos(sectorAngle) * y0);   // ny
		norms.push_back(z0);  // nz
	}

	return norms;
}

/* Get indices for a cylinder */
////////////////////////////////
vector<GLuint> Mesh::getCylinderIndices(float stackCount, float sectorCount, int baseVertexIndex, int topVertexIndex)
{
	vector<GLuint> idx;
	GLuint k1, k2;
	// Get indices for sides
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // Bebinning of current stack
		k2 = k1 + sectorCount + 1;      // Beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles
			idx.push_back(k1);
			idx.push_back(k1 + 1);
			idx.push_back(k2);

			idx.push_back(k2);
			idx.push_back(k1 + 1);
			idx.push_back(k2 + 1);
		}
	}

	// Keep track of where the base top indices start
	unsigned int baseIndex = (unsigned int)idx.size();

	// Get indices for base
	for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1))
		{
			idx.push_back(baseVertexIndex);
			idx.push_back(k + 1);
			idx.push_back(k);
		}
		else // Last triangle
		{
			idx.push_back(baseVertexIndex);
			idx.push_back(baseVertexIndex + 1);
			idx.push_back(k);
		}
	}

	// Keep track of where the base top indices start
	unsigned int topIndex = (unsigned int)idx.size();

	for (int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1))
		{
			idx.push_back(topVertexIndex);
			idx.push_back(k);
			idx.push_back(k + 1);
		}
		else
		{
			idx.push_back(topVertexIndex);
			idx.push_back(k);
			idx.push_back(topVertexIndex + 1);
		}
	}

	return idx;
}

/* Draw the plane */
////////////////////
void Mesh::RenderPlane(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	// Set shader
	glUseProgram(shaderId);

	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0, 1.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	// Create a perspective projection for clip space
	if (perspective)
	{
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, nVertices);

	// Deactivate VAO
	glBindVertexArray(0);
	glUseProgram(0);
}

/* Draw the cube that creates the scene's box */
////////////////////////////////////////////////
void Mesh::RenderBox(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	// Set shader
	glUseProgram(shaderId);

	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(35.0f), glm::vec3(0.0, -1.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(0.75f, 0.0f, -1.0f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	// Create a perspective projection for clip space
	if (perspective)
	{
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, nVertices);

	// Deactivate VAO
	glBindVertexArray(0);

	glUseProgram(0);
}

/* Draw the cube that create's the scene's notepad */
/////////////////////////////////////////////////////
void Mesh::RenderNotepad(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	// Set shader
	glUseProgram(shaderId);

	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(0.0, 1.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(-2.0f, 0.0f, 2.0f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	// Create a perspective projection for clip space
	if (perspective)
	{
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, nVertices);

	// Deactivate VAO
	glBindVertexArray(0);

	glUseProgram(0);
}

/* Draw the sphere AND the light sources */
///////////////////////////////////////////
void Mesh::RenderSphere(GLuint shaderId, GLuint lightShader, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	/*
	 * Draw the sphere
	 */

	// Set shader
	glUseProgram(shaderId);

	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(45.0f), glm::vec3(1.0, 0.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.49f, -1.5f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	if (perspective)
	{
		// Create a perspective projection for clip space
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		// Create orthographic projection
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	/*
	 * Draw the directional light
	 */

	glUseProgram(lightShader);

	//Transform the smaller cube used as a visual que for the light source
	model = glm::translate(gKeyLightPosition) * glm::scale(gLightScale);

	// Get location of uniform variables in vertex shader
	GLint modelLoc = glGetUniformLocation(shaderId, "model");
	GLint viewLoc = glGetUniformLocation(shaderId, "view");
	GLint projLoc = glGetUniformLocation(shaderId, "projection");

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(lightShader, "model");
	viewLoc = glGetUniformLocation(lightShader, "view");
	projLoc = glGetUniformLocation(lightShader, "projection");

	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	/*
	 * Draw the point light
	 */

	glUseProgram(lightShader);

	//Transform the smaller cube used as a visual que for the light source
	model = glm::translate(gFillLightPosition) * glm::scale(gLightScale);

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(lightShader, "model");
	viewLoc = glGetUniformLocation(lightShader, "view");
	projLoc = glGetUniformLocation(lightShader, "projection");

	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	/*
	 * Draw the point light
	 */

	glUseProgram(lightShader);

	//Transform the smaller cube used as a visual que for the light source
	model = glm::translate(gFillLightPosition2) * glm::scale(gLightScale);

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(lightShader, "model");
	viewLoc = glGetUniformLocation(lightShader, "view");
	projLoc = glGetUniformLocation(lightShader, "projection");

	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	glUseProgram(0);
}

/* Draw the cylinder that create's the pencil body */
/////////////////////////////////////////////////////
void Mesh::RenderPencilBody(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	glUseProgram(shaderId);
	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(-1.5f, 0.4f, 0.0f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	// Create a perspective projection for clip space
	if (perspective)
	{
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);
	glUseProgram(0);
}

/* Draw the cylinder (cone) the create's the pencil tip */
//////////////////////////////////////////////////////////
void Mesh::RenderPencilTip(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords)
{
	glUseProgram(shaderId);
	// Scale the object
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// Rotate object
	glm::mat4 rotation = glm::rotate(glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Move object 
	glm::mat4 translation = glm::translate(glm::vec3(0.962f, 0.4f, 0.434f));
	// Model matrix
	glm::mat4 model = translation * rotation * scale;

	// Transform the camera for view space
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection;

	// Create a perspective projection for clip space
	if (perspective)
	{
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(orthoCoords[0], orthoCoords[1], orthoCoords[2], orthoCoords[3], 0.0f, 1000.0f);
	}

	// Set uniform variables for the shaders
	setUniforms(model, view, projection, shaderId, camera, true);

	// Activate VBOs within VAO
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Draw
	glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);
	glUseProgram(0);
}

/* Set the uniform variables for an object to render */
///////////////////////////////////////////////////////
void Mesh::setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint shaderId, Camera camera, bool hasTexture)
{
	// Get location of uniform variables in vertex shader
	GLint modelLoc = glGetUniformLocation(shaderId, "model");
	GLint viewLoc = glGetUniformLocation(shaderId, "view");
	GLint projLoc = glGetUniformLocation(shaderId, "projection");

	// Pass the matrices to the vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Get location of uniform variables in fragment shader
	GLint objectColorLoc = glGetUniformLocation(shaderId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(shaderId, "viewPosition");
	GLint hasTextureLoc = glGetUniformLocation(shaderId, "hasTexture");

	// Handle multiple lights
	// Point light
	glUniform3f(glGetUniformLocation(shaderId, "lights[0].position"), gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);
	glUniform3f(glGetUniformLocation(shaderId, "lights[0].color"), gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
	glUniform1f(glGetUniformLocation(shaderId, "lights[0].intensity"), 1.0f);
	// Point light
	glUniform3f(glGetUniformLocation(shaderId, "lights[1].position"), gFillLightPosition2.x, gFillLightPosition2.y, gFillLightPosition2.z);
	glUniform3f(glGetUniformLocation(shaderId, "lights[1].color"), gFillLightColor2.r, gFillLightColor2.g, gFillLightColor2.b);
	glUniform1f(glGetUniformLocation(shaderId, "lights[1].intensity"), 1.0f);
	// Directional light
	glUniform3f(glGetUniformLocation(shaderId, "lights[2].direction"), gKeyLightDirection.x, gKeyLightDirection.y, gKeyLightDirection.z);
	glUniform3f(glGetUniformLocation(shaderId, "lights[2].color"), gKeyLightColor.r, gKeyLightColor.g, gKeyLightColor.b);


	// Pass the data to the fragment shader
	glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
	const glm::vec3 cameraPosition = camera.Position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniform1i(hasTextureLoc, hasTexture);
}

/* Reset the mesh */
////////////////////
void Mesh::ClearMesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	nVertices = 0;
}

/* Destructor */
////////////////
Mesh::~Mesh()
{
	ClearMesh();
}
