#pragma once
#include "cgPoint3D.h"
#include <vector>
#include <GL\glew.h>
#include <GL/glut.h>
using namespace std;

//extern mat4 projection;
//extern mat4 projectionMat;
//extern mat4 viewMat;
//extern mat4 model;

class cgCylinder
{
public:
	cgCylinder(void);
	~cgCylinder(void);

	void InitData(float r,float h);
	void Render();
	void SetPos(cgPoint3D pos);
	void SetTexture(GLuint texture);
	void SetLightPos(glm::vec3 lightPos);

	void LoadShader(const char* vsfileName, const char* fsfileName);

	cgProgram prog;
	cgProgram textprog;

	unsigned int vboHandle[3];
	unsigned int vaoHandle;

	mat4 projectionMat;
	mat4 viewMat;
	mat4 model;
	vec3 color;

	vec3 viewPos;
	vec3 lightcolor;
	vec3 lightPos;

	vec3 Ka;//环境光
	vec3 Kd;//漫反射光
	vec3 Ks;//镜面反射光

	int shiness;//高光系数

private:
	float fR;
	cgPoint3D ptPos;
	//vector<cgTriangle> vTris;
	GLuint textureID;

	int iCount;//三角形个数
};
