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

struct Point
{
	float x, y, z;
};

class Curved
{
public:
	Curved(void);
	~Curved(void);

	void InitData();
	void Render();
	void SetPos(cgPoint3D pos);
	void SetTexture(GLuint texture);
	void SetLightPos(glm::vec3 lightPos);
	Point curve(float t);

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

	vec3 Ka;//������
	vec3 Kd;//�������
	vec3 Ks;//���淴���

	int shiness;//�߹�ϵ��

private:
	float fR;
	cgPoint3D ptPos;
	//vector<cgTriangle> vTris;
	GLuint textureID;

	int iCount;//�����θ���
	int vertexCount;
};