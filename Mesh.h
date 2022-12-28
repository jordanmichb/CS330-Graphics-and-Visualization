#pragma once

#include "dependencies/camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

using namespace std;

class Mesh
{
public:
	Mesh();

	void CreatePlane();
	void CreateCube(float length, float height, float width);
	void CreateSphere(float radius, float sectorCount, float stackCount);
	void CreateCylinder(float baseRadius, float topRadius, float sectorCount, float height, float stackCount);
	void RenderPlane(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void RenderBox(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void RenderNotepad(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void RenderSphere(GLuint shaderId, GLuint lightShader, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void RenderPencilBody(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void RenderPencilTip(GLuint shaderId, GLuint textureId, GLint width, GLint height, Camera camera, bool perspective, GLfloat* orthoCoords);
	void ClearMesh();

	~Mesh();

private:
	vector<GLfloat> getUnitCircleVertices(float sectorStep, float sectorCount);
	vector<GLfloat> getCylinderNormals(float sectorStep, float sectorCount, float zAngle);
	vector<GLuint> getCylinderIndices(float stackCount, float sectorCount, int baseVertexIndex, int topIndexVertex);
	void setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint shaderId, Camera camera, bool hasTexture);

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint nVertices;
};

