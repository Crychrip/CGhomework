#include "cgProgram.h" 
//#include "lightPos.h"
#include "cgCube.h"
#include <math.h>
#include <GL/glew.h>//OpenGL��
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

	Ka = vec3(0.0, 0.0, 0.3);//������
	Kd = vec3(0.0, 0.0, 0.4);//�������
	Ks = vec3(1.0, 1.0, 1.0);//���淴���

	shiness = 20;//�߹�ϵ��

	textureID = 0;
}


cgCube::~cgCube(void)
{
	glDeleteBuffers(2, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������
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

	//���嶥������

	float* vertc = new float[3 * 3 * 12];
	float* normal = new float[3 * 3 * 12];//��������
	float* textcor = new float[3 * 2 * 12];//��������

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

	glGenBuffers(3, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 12 * sizeof(float), vertc, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * 12 * sizeof(float), textcor, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ����������루���1���������Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//���������ݣ����������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 12 * sizeof(float), normal, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��㷨�������루���2�������������Զ�Ӧ
	glEnableVertexAttribArray(2);  //  // ����Vertex��ɫ�����������2��ȡ�Դ����ݡ�

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
	//������Ҫ��shader�����Զ�̬�л���ͬ��shader

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

	//�ر����õ�shader
	prog.Unuse();

}