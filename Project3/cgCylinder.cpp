#include "cgProgram.h" 
//#include "lightPos.h"
#include "cgCylinder.h"

#include <math.h>
#include <GL/glew.h>//OpenGL��
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


cgCylinder::cgCylinder(void)
{
	ptPos = cgPoint3D();
	using glm::vec4;
	model = glm::mat4(1.0);

	textureID = 0;

	color = glm::vec3(1, 1, 0);

	viewPos = glm::vec3(2, 2, 5);

	lightcolor = glm::vec3(1, 1, 1);

	lightPos = glm::vec3(0.0f, 2.0f, -1.0f);

	Ka = vec3(0.0, 0.0, 0.3);//������
	Kd = vec3(0.0, 0.0, 0.4);//�������
	Ks = vec3(1.0, 1.0, 1.0);//���淴���

	shiness = 20;//�߹�ϵ��

	textureID = 0;
}


cgCylinder::~cgCylinder(void)
{
	glDeleteBuffers(2, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������
}

void cgCylinder::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCylinder::SetLightPos(glm::vec3 lightPos) {
	this->lightPos = lightPos;
}

void cgCylinder::InitData(float r,float h)
{
	fR = r;

	//vTris.clear();
	//��������������ԭ�������������
	int m = 40;//ˮƽ����Բ��ɢ������α���
	int n = 40;//��ֱ������Ƭ����

	iCount = m * n * 2;
	//���嶥������
	float* vertc = new float[3 * 3 * iCount];
	float* normal = new float[3 * 3 * iCount];//��������
	float* textcor = new float[3 * 2 * iCount];//��������

	float dalpha = 2 * PI / m;
	//float dbeta = PI / n;
	//float beta = -PI / 2.0f;
	float dy = h / n;
	float y = -h / 2;

	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	vec3 vnormal;

	for (int i = 0; i < n; i++)
	{
		float alpha = 0.0f;

		//float z1 = fR * sin(beta);
		//float z2 = fR * sin(beta + dbeta);
		float z1 = y;
		float z2 = y + dy;

		//float r0 = fR * cos(beta);
		//float r1 = fR * cos(beta + dbeta);

		float t1 = y / h + 0.5f;
		float t2 = (y+ dy) / h + 0.5f;

		y += dy;
		for (int j = 0; j < m; j++)
		{
			float x01 = fR * cos(alpha);
			float y01 = fR * sin(alpha);

			float x02 = fR * cos(alpha + dalpha);
			float y02 = fR * sin(alpha + dalpha);

			float x11 = fR * cos(alpha);
			float y11 = fR * sin(alpha);

			float x12 = fR * cos(alpha + dalpha);
			float y12 = fR * sin(alpha + dalpha);

			float s1 = alpha / (2.0f * PI);
			float s2 = (alpha + dalpha) / (2.0f * PI);
			//�ı��ηֳ�2��������
			//������1��������
			vertc[vindex++] = x01; vertc[vindex++] = y01; vertc[vindex++] = z1;
			//������
			vnormal = glm::normalize(vec3(x01, y01, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			//��������
			textcor[tindex++] = s1; textcor[tindex++] = t1;

			vertc[vindex++] = x02; vertc[vindex++] = y02; vertc[vindex++] = z1;
			vnormal = glm::normalize(vec3(x02, y02, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t1;

			vertc[vindex++] = x11; vertc[vindex++] = y11; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x11, y11, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s1; textcor[tindex++] = t2;

			//��2��������
			vertc[vindex++] = x02; vertc[vindex++] = y02; vertc[vindex++] = z1;
			vnormal = glm::normalize(vec3(x02, y02, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t1;

			vertc[vindex++] = x12; vertc[vindex++] = y12; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x12, y12, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s2; textcor[tindex++] = t2;

			vertc[vindex++] = x11; vertc[vindex++] = y11; vertc[vindex++] = z2;
			vnormal = glm::normalize(vec3(x11, y11, 0));
			normal[nindex++] = vnormal.x; normal[nindex++] = vnormal.y; normal[nindex++] = vnormal.z;
			textcor[tindex++] = s1; textcor[tindex++] = t2;

			alpha += dalpha;
		}
	}
	glGenBuffers(3, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), vertc, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * iCount * sizeof(float), textcor, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ����������루���1���������Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//���������ݣ����������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * iCount * sizeof(float), normal, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��㷨�������루���2�������������Զ�Ӧ
	glEnableVertexAttribArray(2);  //  // ����Vertex��ɫ�����������2��ȡ�Դ����ݡ�

	glBindVertexArray(0);

	delete[]vertc;
	delete[]normal;
	delete[]textcor;
}

void cgCylinder::SetTexture(GLuint texture)
{
	textureID = texture;
}

void cgCylinder::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.CompileShader(vsfileName);
	prog.CompileShader(fsfileName);
	prog.Link();

}

void cgCylinder::Render()
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
	glDrawArrays(GL_TRIANGLES, 0, 3 * iCount);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//�ر����õ�shader
	prog.Unuse();

}