// ProjectOne.cpp : 定义控制台应用程序的入口点。
//
#include "cgProgram.h" //一定要放在glew之前

#include <GL/glew.h>//OpenGL扩展库
#include <GL/glut.h>//OpenGL工具库
#include <stdio.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using glm::vec4;

cgProgram prog;

mat4 projection;

// 定义Π
#define PI 3.14159265

unsigned int vboHandle[2];
unsigned int vaoHandle;

unsigned int vboHandle2[2];
unsigned int vaoHandle2;

unsigned int vboHandle3[2];
unsigned int vaoHandle3;

unsigned int vboHandle4[2];
unsigned int vaoHandle4;

unsigned int vboHandle5[2];
unsigned int vaoHandle5;

unsigned int vboHandle6[2];
unsigned int vaoHandle6;

// 定义坐标结构体
struct Vertex {
	float x;
	float y;
};

// 获取外圆五角星的五顶点(参数：大圆圆心坐标以及半径)
Vertex* getExternalVertex(Vertex Center_Vertex, double R) {
	// 定义Vertex结构体数组，包括五个顶点和一个圆心
	Vertex* externalVertex = new Vertex[6];
	float alpha = 2 * PI / 5;
	float thet = PI / 10;
	float bet = 2 * PI / 10;
	// 圆心坐标
	externalVertex[0].x = Center_Vertex.x, externalVertex[0].y = Center_Vertex.y;
	// 其余五个顶点
	externalVertex[1].x = Center_Vertex.x, externalVertex[1].y = Center_Vertex.y + R;
	externalVertex[2].x = Center_Vertex.x + R * cos(thet), externalVertex[2].y = Center_Vertex.y + R * sin(thet);
	externalVertex[3].x = Center_Vertex.x + R * sin(bet), externalVertex[3].y = Center_Vertex.y - R * cos(bet);
	externalVertex[4].x = Center_Vertex.x - R * sin(bet), externalVertex[4].y = Center_Vertex.y - R * cos(bet);
	externalVertex[5].x = Center_Vertex.x - R * cos(thet), externalVertex[5].y = Center_Vertex.y + R * sin(thet);

	return externalVertex;
}

// 获取内圆五角星的五顶点(参数：小圆圆心坐标以及半径)
Vertex* getInternalVertex(Vertex Center_Vertex, double r) {
	// 定义Vertex结构体数组，包括五个顶点和一个圆心
	Vertex* internalVertex = new Vertex[6];
	float alpha = 2 * PI / 5;
	float thet = PI / 10;
	float bet = (PI - alpha) / 2;
	// 圆心坐标
	internalVertex[0].x = Center_Vertex.x, internalVertex[0].y = Center_Vertex.y;
	// 其余五个顶点
	internalVertex[1].x = Center_Vertex.x + r * cos(bet), internalVertex[1].y = Center_Vertex.y + r * sin(bet);
	internalVertex[2].x = Center_Vertex.x + r * cos(thet), internalVertex[2].y = Center_Vertex.y - r * sin(thet);//
	internalVertex[3].x = Center_Vertex.x, internalVertex[3].y = Center_Vertex.y - r;//
	internalVertex[4].x = Center_Vertex.x - r * cos(thet), internalVertex[4].y = Center_Vertex.y - r * sin(thet);
	internalVertex[5].x = Center_Vertex.x - r * cos(bet), internalVertex[5].y = Center_Vertex.y + r * sin(bet);

	return internalVertex;
}

// 设置背景颜色
void setBackgroundColor(void) {
	glClearColor(0.2, 0.3, 0.3, 0.5);
}

void initTriangle(void)
{
	Vertex Center_Vertex;
	Center_Vertex.x = 500, Center_Vertex.y = 150;
	double R = 100;
	double r = 40;

	Vertex* externalVertex = getExternalVertex(Center_Vertex, R);
	Vertex* internalVertex = getInternalVertex(Center_Vertex, r);

	float* vertc1 = new float[10 * 2];
	float* color1 = new float[10 * 3];

	int countv1 = 0;
	int countc1 = 0;


	for (int i = 1; i <= 5; i++) 
	{
		vertc1[countv1++] = externalVertex[i].x;
		vertc1[countv1++] = externalVertex[i].y;
		vertc1[countv1++] = internalVertex[i].x;
		vertc1[countv1++] = internalVertex[i].y;
	}

	for (int j = 1; j <= 10; j++) 
	{
		color1[countc1++] = 1.0f;
		color1[countc1++] = 0.0f;
		color1[countc1++] = 0.0f;
	}

	glGenBuffers(2, vboHandle);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 10 * 2 * sizeof(float), vertc1, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 10 * 3 * sizeof(float), color1, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc1;
	delete[] color1;
}

void fillstar(void)
{
	Vertex Center_Vertex;
	Center_Vertex.x = 500, Center_Vertex.y = 350;
	double R = 100;
	double r = 40;

	Vertex* externalVertex = getExternalVertex(Center_Vertex, R);
	Vertex* internalVertex = getInternalVertex(Center_Vertex, r);

	float* vertc2 = new float[30 * 2];
	float* color2 = new float[30 * 3];

	int countv2 = 0;
	int countc2 = 0;

	for (int i = 1; i < 5; i++)
	{
		vertc2[countv2++] = externalVertex[0].x;
		vertc2[countv2++] = externalVertex[0].y;
		vertc2[countv2++] = externalVertex[i].x;
		vertc2[countv2++] = externalVertex[i].y;
		vertc2[countv2++] = internalVertex[i].x;
		vertc2[countv2++] = internalVertex[i].y;

		vertc2[countv2++] = externalVertex[0].x;
		vertc2[countv2++] = externalVertex[0].y;
		vertc2[countv2++] = externalVertex[i + 1].x;
		vertc2[countv2++] = externalVertex[i + 1].y;
		vertc2[countv2++] = internalVertex[i].x;
		vertc2[countv2++] = internalVertex[i].y;
	}
	vertc2[countv2++] = externalVertex[0].x;
	vertc2[countv2++] = externalVertex[0].y;
	vertc2[countv2++] = externalVertex[5].x;
	vertc2[countv2++] = externalVertex[5].y;
	vertc2[countv2++] = internalVertex[5].x;
	vertc2[countv2++] = internalVertex[5].y;

	vertc2[countv2++] = externalVertex[0].x;
	vertc2[countv2++] = externalVertex[0].y;
	vertc2[countv2++] = externalVertex[1].x;
	vertc2[countv2++] = externalVertex[1].y;
	vertc2[countv2++] = internalVertex[5].x;
	vertc2[countv2++] = internalVertex[5].y;

	for (int j = 1; j <= 30; j++)
	{
		color2[countc2++] = 1.0f;
		color2[countc2++] = 0.0f;
		color2[countc2++] = 0.0f;
	}

	glGenBuffers(2, vboHandle2);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle2);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle2);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle2[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 30 * 2 * sizeof(float), vertc2, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle2[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 30 * 3 * sizeof(float), color2, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc2;
	delete[] color2;
}

void fillstartwo(void)
{
	Vertex Center_Vertex;
	Center_Vertex.x = 500, Center_Vertex.y = 550;
	double R = 100;
	double r = 40;

	Vertex* externalVertex = getExternalVertex(Center_Vertex, R);
	Vertex* internalVertex = getInternalVertex(Center_Vertex, r);

	float* vertc3 = new float[30 * 2];
	float* color3 = new float[30 * 3];

	int countv3 = 0;
	int countc3 = 0;

	for (int i = 1; i < 5; i++)
	{
		vertc3[countv3++] = externalVertex[0].x;
	    vertc3[countv3++] = externalVertex[0].y;
		vertc3[countv3++] = externalVertex[i].x;
		vertc3[countv3++] = externalVertex[i].y;
		vertc3[countv3++] = internalVertex[i].x;
		vertc3[countv3++] = internalVertex[i].y;

		vertc3[countv3++] = externalVertex[0].x;
		vertc3[countv3++] = externalVertex[0].y;
		vertc3[countv3++] = externalVertex[i + 1].x;
		vertc3[countv3++] = externalVertex[i + 1].y;
		vertc3[countv3++] = internalVertex[i].x;
		vertc3[countv3++] = internalVertex[i].y;
	}
	vertc3[countv3++] = externalVertex[0].x;
	vertc3[countv3++] = externalVertex[0].y;
	vertc3[countv3++] = externalVertex[5].x;
	vertc3[countv3++] = externalVertex[5].y;
	vertc3[countv3++] = internalVertex[5].x;
	vertc3[countv3++] = internalVertex[5].y;

	vertc3[countv3++] = externalVertex[0].x;
	vertc3[countv3++] = externalVertex[0].y;
	vertc3[countv3++] = externalVertex[1].x;
	vertc3[countv3++] = externalVertex[1].y;
	vertc3[countv3++] = internalVertex[5].x;
	vertc3[countv3++] = internalVertex[5].y;

	for (int j = 1; j <= 5; j++)
	{
		for (int m = 1; m <= 3; m++)
		{
			color3[countc3++] = 1.0f;
			color3[countc3++] = 0.0f;
			color3[countc3++] = 0.0f;
		}
		for (int n = 1; n <= 3; n++) 
		{
			color3[countc3++] = 0.0f;
			color3[countc3++] = 1.0f;
			color3[countc3++] = 1.0f;
		}
	}

	glGenBuffers(2, vboHandle3);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle3);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle3);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle3[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 30 * 2 * sizeof(float), vertc3, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle3[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 30 * 3 * sizeof(float), color3, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc3;
	delete[] color3;
}

void flag(void)
{
	float a = 200.0f, b = 300.0f;
	float R = a / 2 / 10 * 3;
	float r = R / 10 * 4;
	float RR = R / 3;
	float rr = RR / 10 * 4;

	float* vertc4 = new float[150 * 2];
	float* color4 = new float[150 * 3];
	int countv4 = 0;
	int countc4 = 0;

	Vertex Center_Vertex1, Center_Vertex2, Center_Vertex3, Center_Vertex4, Center_Vertex5;
	Center_Vertex1.x = 150, Center_Vertex1.y = 250;
	Center_Vertex2.x = 200, Center_Vertex2.y = 280;
	Center_Vertex3.x = 220, Center_Vertex3.y = 260;
	Center_Vertex4.x = 220, Center_Vertex4.y = 230;
	Center_Vertex5.x = 200, Center_Vertex5.y = 210;

	Vertex* externalVertex1 = getExternalVertex(Center_Vertex1, R); Vertex* internalVertex1 = getInternalVertex(Center_Vertex1, r);
	Vertex* externalVertex2 = getExternalVertex(Center_Vertex2, RR); Vertex* internalVertex2 = getInternalVertex(Center_Vertex2, rr);
	Vertex* externalVertex3 = getExternalVertex(Center_Vertex3, RR); Vertex* internalVertex3 = getInternalVertex(Center_Vertex3, rr);
	Vertex* externalVertex4 = getExternalVertex(Center_Vertex4, RR); Vertex* internalVertex4 = getInternalVertex(Center_Vertex4, rr);
	Vertex* externalVertex5 = getExternalVertex(Center_Vertex5, RR); Vertex* internalVertex5 = getInternalVertex(Center_Vertex5, rr);

	for (int i = 1; i < 5; i++)
	{
		vertc4[countv4++] = externalVertex1[0].x; vertc4[countv4++] = externalVertex1[0].y;
		vertc4[countv4++] = externalVertex1[i].x; vertc4[countv4++] = externalVertex1[i].y;
		vertc4[countv4++] = internalVertex1[i].x; vertc4[countv4++] = internalVertex1[i].y;

		vertc4[countv4++] = externalVertex1[0].x; vertc4[countv4++] = externalVertex1[0].y;
		vertc4[countv4++] = externalVertex1[i + 1].x; vertc4[countv4++] = externalVertex1[i + 1].y;
		vertc4[countv4++] = internalVertex1[i].x; vertc4[countv4++] = internalVertex1[i].y;
	}
	vertc4[countv4++] = externalVertex1[0].x; vertc4[countv4++] = externalVertex1[0].y;
	vertc4[countv4++] = externalVertex1[5].x; vertc4[countv4++] = externalVertex1[5].y;
	vertc4[countv4++] = internalVertex1[5].x; vertc4[countv4++] = internalVertex1[5].y;

	vertc4[countv4++] = externalVertex1[0].x; vertc4[countv4++] = externalVertex1[0].y;
	vertc4[countv4++] = externalVertex1[1].x; vertc4[countv4++] = externalVertex1[1].y;
	vertc4[countv4++] = internalVertex1[5].x; vertc4[countv4++] = internalVertex1[5].y;
	for (int i = 1; i < 5; i++)
	{
		vertc4[countv4++] = externalVertex2[0].x; vertc4[countv4++] = externalVertex2[0].y;
		vertc4[countv4++] = externalVertex2[i].x; vertc4[countv4++] = externalVertex2[i].y;
		vertc4[countv4++] = internalVertex2[i].x; vertc4[countv4++] = internalVertex2[i].y;

		vertc4[countv4++] = externalVertex2[0].x; vertc4[countv4++] = externalVertex2[0].y;
		vertc4[countv4++] = externalVertex2[i + 1].x; vertc4[countv4++] = externalVertex2[i + 1].y;
		vertc4[countv4++] = internalVertex2[i].x; vertc4[countv4++] = internalVertex2[i].y;
	}
	vertc4[countv4++] = externalVertex2[0].x; vertc4[countv4++] = externalVertex2[0].y;
	vertc4[countv4++] = externalVertex2[5].x; vertc4[countv4++] = externalVertex2[5].y;
	vertc4[countv4++] = internalVertex2[5].x; vertc4[countv4++] = internalVertex2[5].y;

	vertc4[countv4++] = externalVertex2[0].x; vertc4[countv4++] = externalVertex2[0].y;
	vertc4[countv4++] = externalVertex2[1].x; vertc4[countv4++] = externalVertex2[1].y;
	vertc4[countv4++] = internalVertex2[5].x; vertc4[countv4++] = internalVertex2[5].y;
	for (int i = 1; i < 5; i++)
	{
		vertc4[countv4++] = externalVertex3[0].x; vertc4[countv4++] = externalVertex3[0].y;
		vertc4[countv4++] = externalVertex3[i].x; vertc4[countv4++] = externalVertex3[i].y;
		vertc4[countv4++] = internalVertex3[i].x; vertc4[countv4++] = internalVertex3[i].y;

		vertc4[countv4++] = externalVertex3[0].x; vertc4[countv4++] = externalVertex3[0].y;
		vertc4[countv4++] = externalVertex3[i + 1].x; vertc4[countv4++] = externalVertex3[i + 1].y;
		vertc4[countv4++] = internalVertex3[i].x; vertc4[countv4++] = internalVertex3[i].y;
	}
	vertc4[countv4++] = externalVertex3[0].x; vertc4[countv4++] = externalVertex3[0].y;
	vertc4[countv4++] = externalVertex3[5].x; vertc4[countv4++] = externalVertex3[5].y;
	vertc4[countv4++] = internalVertex3[5].x; vertc4[countv4++] = internalVertex3[5].y;

	vertc4[countv4++] = externalVertex3[0].x; vertc4[countv4++] = externalVertex3[0].y;
	vertc4[countv4++] = externalVertex3[1].x; vertc4[countv4++] = externalVertex3[1].y;
	vertc4[countv4++] = internalVertex3[5].x; vertc4[countv4++] = internalVertex3[5].y;
	for (int i = 1; i < 5; i++)
	{
		vertc4[countv4++] = externalVertex4[0].x; vertc4[countv4++] = externalVertex4[0].y;
		vertc4[countv4++] = externalVertex4[i].x; vertc4[countv4++] = externalVertex4[i].y;
		vertc4[countv4++] = internalVertex4[i].x; vertc4[countv4++] = internalVertex4[i].y;

		vertc4[countv4++] = externalVertex4[0].x; vertc4[countv4++] = externalVertex4[0].y;
		vertc4[countv4++] = externalVertex4[i + 1].x; vertc4[countv4++] = externalVertex4[i + 1].y;
		vertc4[countv4++] = internalVertex4[i].x; vertc4[countv4++] = internalVertex4[i].y;
	}
	vertc4[countv4++] = externalVertex4[0].x; vertc4[countv4++] = externalVertex4[0].y;
	vertc4[countv4++] = externalVertex4[5].x; vertc4[countv4++] = externalVertex4[5].y;
	vertc4[countv4++] = internalVertex4[5].x; vertc4[countv4++] = internalVertex4[5].y;

	vertc4[countv4++] = externalVertex4[0].x; vertc4[countv4++] = externalVertex4[0].y;
	vertc4[countv4++] = externalVertex4[1].x; vertc4[countv4++] = externalVertex4[1].y;
	vertc4[countv4++] = internalVertex4[5].x; vertc4[countv4++] = internalVertex4[5].y;
	for (int i = 1; i < 5; i++)
	{
		vertc4[countv4++] = externalVertex5[0].x; vertc4[countv4++] = externalVertex5[0].y;
		vertc4[countv4++] = externalVertex5[i].x; vertc4[countv4++] = externalVertex5[i].y;
		vertc4[countv4++] = internalVertex5[i].x; vertc4[countv4++] = internalVertex5[i].y;

		vertc4[countv4++] = externalVertex5[0].x; vertc4[countv4++] = externalVertex5[0].y;
		vertc4[countv4++] = externalVertex5[i + 1].x; vertc4[countv4++] = externalVertex5[i + 1].y;
		vertc4[countv4++] = internalVertex5[i].x; vertc4[countv4++] = internalVertex5[i].y;
	}
	vertc4[countv4++] = externalVertex5[0].x; vertc4[countv4++] = externalVertex5[0].y;
	vertc4[countv4++] = externalVertex5[5].x; vertc4[countv4++] = externalVertex5[5].y;
	vertc4[countv4++] = internalVertex5[5].x; vertc4[countv4++] = internalVertex5[5].y;

	vertc4[countv4++] = externalVertex5[0].x; vertc4[countv4++] = externalVertex5[0].y;
	vertc4[countv4++] = externalVertex5[1].x; vertc4[countv4++] = externalVertex5[1].y;
	vertc4[countv4++] = internalVertex5[5].x; vertc4[countv4++] = internalVertex5[5].y;

	for (int j = 1; j <= 150; j++)
	{
		color4[countc4++] = 255.0f;
		color4[countc4++] = 255.0f;
		color4[countc4++] = 0.0f;
	}

	glGenBuffers(2, vboHandle4);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle4);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle4);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle4[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 150 * 2 * sizeof(float), vertc4, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle4[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 150 * 3 * sizeof(float), color4, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc4;
	delete[] color4;
}

void rectangle(void)
{
	float a = 200.0f, b = 300.0f;
	float* vertc5 = new float[6 * 2];
	float* color5 = new float[6 * 3];
	int countv5 = 0;
	int countc5 = 0;
	vertc5[countv5++] = 100.0f; vertc5[countv5++] = 100.0f;
	vertc5[countv5++] = 100.0f; vertc5[countv5++] = 100.0f + a;
	vertc5[countv5++] = 100.0f + b; vertc5[countv5++] = 100.0f;
	vertc5[countv5++] = 100.0f; vertc5[countv5++] = 100.0f + a;
	vertc5[countv5++] = 100.0f + b; vertc5[countv5++] = 100.0f;
	vertc5[countv5++] = 100.0f + b; vertc5[countv5++] = 100.0f + a;
	for (int i = 1; i <= 6; i++)
	{
		color5[countc5++] = 1.0f;
		color5[countc5++] = 0.0f;
		color5[countc5++] = 0.0f;
	}
	glGenBuffers(2, vboHandle5);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle5);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle5);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle5[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertc5, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle5[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), color5, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc5;
	delete[] color5;
}

//int totalPoints;
//float startAngle = 0.0f;

//void wave()
//{
//	const int W = 300, H = 200;
//	float points[W * 2 * 5];
//	float upperLeftX = 100.0f;
//	float upperLeftY = 100.0f;
//	float xSpan = 1.6f;
//	float ySpan = 1.6f;
//	float dx = xSpan / W;
//	float dy = ySpan / H;
//
//	// 正弦波相关参数
//	float angleSpan = 8 * PI;  // 相位跨度
//	float deltaA = angleSpan / W;  // 单个点的相位增量
//	float heightSpan = 0.02;  // 振幅
//
//	totalPoints = W * 2;
//
//	for (int i = 0; i < W; i++) {
//		// 上
//		points[i * 5 + 0] = upperLeftX + i * dx;
//		points[i * 5 + 1] = upperLeftY + glm::sin(startAngle + i * deltaA) * heightSpan;
//		points[i * 5 + 2] = 1.0f;
//		points[i * 5 + 3] = ((float)i) / ((float)W);
//		points[i * 5 + 4] = 0.0f;
//		// 下
//		int dn = (i + W + H) * 5;
//		points[dn + 0] = upperLeftX + xSpan - i * dx;
//		points[dn + 1] = upperLeftY - ySpan + glm::sin(startAngle + (W - i) * deltaA) * heightSpan;
//		points[dn + 2] = 0.0f;
//		points[dn + 3] = 1.0f - ((float)i) / ((float)W);
//		points[dn + 4] = 1.0f;
//	}
//	glGenBuffers(2, vboHandle6);//生成缓冲区，数据
//
//	glGenVertexArrays(1, &vaoHandle6);//生成顶点数组，与vertex shader绑定
//	glBindVertexArray(vaoHandle6);//可以多个顶点数组，使用时需要激活哪一个
//
//	glBindBuffer(GL_ARRAY_BUFFER, vboHandle6[0]);//激活（绑定）当前缓冲区
//	glBufferData(GL_ARRAY_BUFFER, W * 5 * 2 * sizeof(float), points, GL_STATIC_DRAW);//内存数据复制到显存
//	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
//	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}

//void Draw() {
//	/**
//	* 生成一个 15 * 10 的顶点数组
//	* 通过 z 轴坐标的变换来实现迎风飘动的波浪感
//	* x1, y1, z1, tx1, ty1, x2, y2, z2, tx2, ty2 ...
//	* 一个顶点 vertex = (x: x坐标, y: y坐标, z: z坐标, tx: 纹理绑定坐标x, ty: 纹理绑定坐标y)
//	* z = sin(A + dn) A 为第一个点的相位，d为增量，n为当前点的列数
//	* d = 预先定义的正弦相位跨度 / 总列数
//	* tx = x
//	* ty = y
//	*/
//
//	const int W = 300, H = 200;
//	float points[(W + H) * 2 * 5];
//	float upperLeftX = 100.0f;
//	float upperLeftY = 100.0f;
//	float xSpan = 1.6f;
//	float ySpan = 1.6f;
//	float dx = xSpan / W;
//	float dy = ySpan / H;
//
//	// 正弦波相关参数
//	float angleSpan = 8 * PI;  // 相位跨度
//	float deltaA = angleSpan / W;  // 单个点的相位增量
//	float heightSpan = 0.02;  // 振幅
//
//	totalPoints = (W + H) * 2;
//
//	for (int i = 0; i < W; i++) {
//		// 上
//		points[i * 5 + 0] = upperLeftX + i * dx;
//		points[i * 5 + 1] = upperLeftY + glm::sin(startAngle + i * deltaA) * heightSpan;
//		points[i * 5 + 2] = 1.0f;
//		points[i * 5 + 3] = ((float)i) / ((float)W);
//		points[i * 5 + 4] = 0.0f;
//		// 下
//		int dn = (i + W + H) * 5;
//		points[dn + 0] = upperLeftX + xSpan - i * dx;
//		points[dn + 1] = upperLeftY - ySpan + glm::sin(startAngle + (W - i) * deltaA) * heightSpan;
//		points[dn + 2] = 0.0f;
//		points[dn + 3] = 1.0f - ((float)i) / ((float)W);
//		points[dn + 4] = 1.0f;
//	}
//
//	for (int i = 0; i < H; i++) {
//		// 左
//		int dn = (i + W + W + H) * 5;
//		points[dn + 0] = upperLeftX;
//		points[dn + 1] = upperLeftY - ySpan + i * dy;
//		points[dn + 2] = 0.0f;
//		points[dn + 3] = 0.0f;
//		points[dn + 4] = 1.0f - ((float)i) / ((float)H);
//		// 右
//		dn = (i + W) * 5;
//		points[dn + 0] = upperLeftX + xSpan;
//		points[dn + 1] = upperLeftY - i * dy;
//		points[dn + 2] = 0.0f;
//		points[dn + 3] = 1.0f;
//		points[dn + 4] = ((float)i) / ((float)H);
//	}
//
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STREAM_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	glEnable(GL_STENCIL_TEST);//开启模板测试
//	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);//设置模板缓冲区更新方式(若通过则按位反转模板值)
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glStencilFunc(GL_ALWAYS, 1, 1);//初始模板位为0，由于一定通过测试，所以全部会被置为1，而重复绘制区域由于画了两次模板位又归0
//	glStencilMask(0x1);//开启模板缓冲区写入
//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_POLYGON, 0, totalPoints);
//
//	glStencilFunc(GL_NOTEQUAL, 0, 1);//模板值不为0就通过，即EAB片段会被舍弃掉，凹多边形就正确画出了
//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//	glStencilMask(0x1);
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_POLYGON, 0, totalPoints);
//
//	glDisable(GL_STENCIL_TEST);
//
//	glDeleteBuffers(1, &VBO);
//	glDeleteVertexArrays(1, &VAO);
//}

int totalPoint;

void DrawDiamond() 
{
	float x = 200.0f, y = 700.0f;
	float R = 300.0f;
	int count = 40;
	float o = 2.0f * PI / count;

	float* points = new float[40 * 2];
	int countp = 0;
	for (int i = 0; i < 40; i++) 
	{
		points[countp++] = x + R * sin(i * o);
		points[countp++] = y + R * cos(i * o);
	}

	float* vertc6 = new float[40 * 39 * 2];
	float* color6 = new float[40 * 39 * 3];
	int countv = 0; int countc = 0;
	for (int i = 0; i < 40; i++) {
		for (int j = i + 1; j < 40; j++) {
			vertc6[countv++] = points[2 * i];
			vertc6[countv++] = points[2 * i + 1];
			vertc6[countv++] = points[2 * j];
			vertc6[countv++] = points[2 * j + 1];
		}
	}
	for (int k = 0; k < 40 * 39 ; k++) {
		color6[countc++] = 0.0f;
		color6[countc++] = 1.0f;
		color6[countc++] = 1.0f;
	}
	glGenBuffers(2, vboHandle6);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle6);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle6);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle6[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 40 * 39 * 2 * sizeof(float), vertc6, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle6[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 40 * 39 * 3 * sizeof(float), color6, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc6;
	delete[] color6;
}

void reshape(int width, int height)//窗口大小变化时执行
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//启用需要的shader，可以动态切换不同的shader
	prog.Use();

	prog.SetUniform("ProjectionMatrix", projection);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_LINE_LOOP, 0, 10);//GL_LINES线段
	glBindVertexArray(0);

	glBindVertexArray(vaoHandle2);
	glDrawArrays(GL_TRIANGLES, 0, 30);//GL_LINES线段
	glBindVertexArray(0);

	glBindVertexArray(vaoHandle3);
	glDrawArrays(GL_TRIANGLES, 0, 30);//GL_LINES线段
	glBindVertexArray(0);

	glBindVertexArray(vaoHandle5);
	glDrawArrays(GL_TRIANGLES, 0, 6);//GL_LINES线段
	glBindVertexArray(0);

	glBindVertexArray(vaoHandle4);
	glDrawArrays(GL_TRIANGLES, 0, 150);//GL_LINES线段
	glBindVertexArray(0);

	glBindVertexArray(vaoHandle6);
	glDrawArrays(GL_LINES, 0, 40 * 39 * 2);//GL_LINES线段
	glBindVertexArray(0);

	/*glUseProgram(startProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, totalPoint * (totalPoint - 1) * 2, GL_UNSIGNED_INT, 0);*/

	//关闭所用的shader
	prog.Unuse();
	//glFlush();//单缓冲时必须要，说明绘图命令（函数）结束
	glutSwapBuffers();//交换缓冲（双缓冲时使用）
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);//设置背景色
}

void LoadShader()
{

	//编译与链接Shader
	prog.CompileShader("Shader/basic.vs");
	prog.CompileShader("Shader/basic.fs");
	prog.Link();
}

void keyboard(unsigned char key, int x, int y)//响应键盘
{
}

//void timerFunc() {
//	if (startAngle <= 0) {
//		startAngle = 2 * PI;
//	}
//	else {
//		startAngle -= 0.002f;
//	}
//	glutPostRedisplay();
//}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 800);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//双缓冲，单缓冲为GLUT_SINGLE
	glutCreateWindow("Project One");

	glewInit();//OpenGL初始化,现代OpenGL需要
	init();

	LoadShader();//加载Shader
	initTriangle();//加载（生成）三角形数据
	fillstar();
	fillstartwo();

	flag();
	rectangle();

	//wave();
	DrawDiamond();

	glutDisplayFunc(display);//图形绘制

	//glutIdleFunc(timerFunc);

	glutReshapeFunc(reshape);//窗口大小变化
	glutKeyboardFunc(keyboard);//键盘交互

	glutMainLoop();

	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象

	return 0;
}
