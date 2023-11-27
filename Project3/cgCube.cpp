#include "cgProgram.h" 
//#include "lightPos.h"
#include "cgCube.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

cgCube::cgCube(void)
{
	ptPos = cgPoint3D();
	using glm::vec4;
	model = glm::mat4(1.0);

	color = glm::vec3(1, 1, 0);

	viewPos = glm::vec3(2, 2, 5);

	lightPos = glm::vec3(0.0f, 2.0f, -1.0f);

	lightcolor = glm::vec3(1, 1, 1);

	Ka = vec3(0.0, 0.0, 0.3);//环境光
	Kd = vec3(0.0, 0.0, 0.4);//漫反射光
	Ks = vec3(1.0, 1.0, 1.0);//镜面反射光

	shiness = 20;//高光系数

	textureID = 0;
}


cgCube::~cgCube(void)
{
	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象
}

void cgCube::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCube::SetLightPos(glm::vec3 lightPos) {
	this->lightPos = lightPos;
}

void cgCube::InitData(float BC)
{
	a=BC;//(-a,-a,-a)(-a,a,-a)(a,-a,-a)(a,a,-a) (-a,-a,a)(a,-a,a)(-a,a,a)(a,a,a)

	//定义顶点数组

	float* vertc = new float[3 * 3 * 12];
	float* normal = new float[3 * 3 * 12];//法向向量
	float* textcor = new float[3 * 2 * 12];//纹理坐标

	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	//vec3 vnormal;

	float v[3 * 3 * 12] = {
		-a, -a, -a,
		 a, -a, -a,  
		 a,  a, -a, 
		 a,  a, -a, 
		-a,  a, -a,
		-a, -a, -a,  

		-a, -a,  a, 
		 a, -a,  a, 
		 a,  a,  a, 
		 a,  a,  a,  
		-a,  a,  a,  
		-a, -a,  a,  

		-a,  a,  a, 
		-a,  a, -a, 
		-a, -a, -a,
		-a, -a, -a,
		-a, -a,  a,
		-a,  a,  a,

		 a,  a,  a,
		 a,  a, -a,
		 a, -a, -a,
		 a, -a, -a,
		 a, -a,  a,
		 a,  a,  a,

		-a, -a, -a,
		 a, -a, -a,
		 a, -a,  a,
		 a, -a,  a,
		-a, -a,  a,
		-a, -a, -a,

		-a,  a, -a,
		 a,  a, -a,
		 a,  a,  a,
		 a,  a,  a,
		-a,  a,  a,
		-a,  a, -a,
	};

	float t[3 * 2 * 12] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	float n[3 * 3 * 12] = {
		-1, -1, -1,
		 1, -1, -1,
		 1,  1, -1,
		 1,  1, -1,
		-1,  1, -1,
		-1, -1, -1,

		-1, -1,  1,
		 1, -1,  1,
		 1,  1,  1,
		 1,  1,  1,
		-1,  1,  1,
		-1, -1,  1,

		-1,  1,  1,
		-1,  1, -1,
		-1, -1, -1,
		-1, -1, -1,
		-1, -1,  1,
		-1,  1,  1,

		 1,  1,  1,
		 1,  1, -1,
		 1, -1, -1,
		 1, -1, -1,
		 1, -1,  1,
		 1,  1,  1,

		-1, -1, -1,
		 1, -1, -1,
		 1, -1,  1,
		 1, -1,  1,
		-1, -1,  1,
		-1, -1, -1,

		-1,  1, -1,
		 1,  1, -1,
		 1,  1,  1,
		 1,  1,  1,
		-1,  1,  1,
		-1,  1, -1,
	};

	for (int i = 0; i < 3 * 3 * 12; i++) {
		vertc[vindex++] = v[i];
		normal[nindex++] = n[i];
	}

	for (int j = 0; j < 3 * 2 * 12; j++) {
		textcor[tindex++] = t[j];
	}

	glGenBuffers(3, vboHandle);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 12 * sizeof(float), vertc, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（纹理）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * 12 * sizeof(float), textcor, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点纹理输入（序号1，纹理）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//缓冲区数据（法向量）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 12 * sizeof(float), normal, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点法向量输入（序号2，法向量）属性对应
	glEnableVertexAttribArray(2);  //  // 允许Vertex着色器中输入变量2读取显存数据。

	glBindVertexArray(0);

	delete[]vertc;
	delete[]normal;
	delete[]textcor;
}

void cgCube::SetTexture(GLuint texture)
{
	textureID = texture;
}

void cgCube::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.CompileShader(vsfileName);
	prog.CompileShader(fsfileName);
	prog.Link();

}

void cgCube::Render()
{
	//启用需要的shader，可以动态切换不同的shader

	prog.Use();

	prog.SetUniform("ProjectionMatrix", projectionMat);
	prog.SetUniform("ViewMatrix", viewMat);
	prog.SetUniform("ModelMatrix", model);

	prog.SetUniform("lightColor", lightcolor);
	prog.SetUniform("lightPos", lightPos);
	prog.SetUniform("viewPos", viewPos);
	prog.SetUniform("Ka", Ka);
	prog.SetUniform("Kd", Kd);
	prog.SetUniform("Ks", Ks);
	prog.SetUniform("shiness", shiness);

	//color = glm::vec3(1, 0, 0);
	prog.SetUniform("objectColor", color);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//关闭所用的shader
	prog.Unuse();

}