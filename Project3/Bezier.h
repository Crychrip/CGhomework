#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include<vector>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

struct Point
{
	float x, y, z;
};

class Bezier
{
public:
	std::vector<Point> Points;  
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	unsigned int TextureID;
	unsigned int ProgramID;

	Bezier();
	Bezier(std::vector<Point>& points);
	~Bezier();
	void SetPoints(std::vector<Point>& points);

	// Drawable
	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetProgram(unsigned int programID);
	void SetTexture(unsigned int textureID);
private:
	unsigned int vao;
	unsigned int vbo;
	int vertexCount;
	void createData();
	Point bezier_curve(float t);
};