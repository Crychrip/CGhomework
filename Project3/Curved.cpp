#include "cgProgram.h" 
#include "Curved.h"
#include <math.h>
#include <GL/glew.h>//OpenGL��
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

Curved::Curved(void)
{
	ptPos = cgPoint3D();
	using glm::vec4;
	model = glm::mat4(1.0);

	viewPos = glm::vec3(2, 2, 5);

	lightPos = glm::vec3(0.0f, 2.0f, -1.0f);

	lightcolor = glm::vec3(1, 1, 1);

	Ka = vec3(0.0, 0.0, 0.3);//������
	Kd = vec3(0.0, 0.0, 0.4);//�������
	Ks = vec3(1.0, 1.0, 1.0);//���淴���

	shiness = 20;//�߹�ϵ��

	textureID = 0;
}


Curved::~Curved(void)
{
	glDeleteBuffers(2, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������
}

void Curved::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

Point Curved::curve(float t) {
	Point res;
	res.x = t;
	res.y = 2.0f * glm::cos(t + glm::pi<float>()) + 2.0f;
	res.z = 0.0f;
	return res;
}

void Curved::InitData()
{

	const int N = 30;

	constexpr float step = PI / N; // ���߲�������

	const int M = 40;  // ��ת��Բ����ɢ������

	vertexCount = N * M * 6;

	float* vertices = new float[vertexCount * 3];
	float* normals = new float[vertexCount * 3];
	float* textcor = new float[vertexCount * 2];
	int verticesIndex = 0;
	int normalsIndex = 0;
	int textorIndex = 0;

	float texX = 0.0f;  // ��������x
	float texY = 0.0f;  // ��������y
	float dtexX = 1.0f / M;  // ����x����
	float dtexY = 1.0f / N;  // ����y����

	float alpha = 0;  // ��ת��
	float dalpha = 360.0f / M;  // �Ƕ�����

	float phi = 0.0f;

	for (int i = 0; i < N; i++) {
		Point p1 = curve(phi);
		Point p2 = curve(phi + step);
		// δ��ת�ĳ�ʼ������1
		float dx1 = 2.0f * glm::sin(phi + glm::pi<float>());
		float dy1 = 1.0f;
		// ������1�յ�����
		float nx1 = p1.x + dx1;
		float ny1 = p1.y + dy1;
		// δ��ת�ĳ�ʼ������2
		float dx2 = 2.0f * glm::sin(phi + step + glm::pi<float>());
		float dy2 = 1.0f;
		// ������2�յ�����
		float nx2 = p2.x + dx2;
		float ny2 = p2.y + dy2;
		// ���·��ĵ�İ뾶
		float r1 = p1.x;
		// ���Ϸ��ĵ�İ뾶
		float r2 = p2.x;
		alpha = 0.0f;
		// �������ϸ�����תһ�ܣ��ڴ�Ĭ��ת��Ϊy��
		for (int j = 0; j < M; j++)
		{
			// ��������������
			// ��һ��������
			// ��һ������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			// ��������
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// �ڶ�������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha)) - r2 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha)) - r2 * glm::sin(glm::radians(alpha));
			// ����������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX + dtexX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));
			// �ڶ���������
			// ��һ������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// �ڶ�������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha + dalpha)) - r1 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha + dalpha)) - r1 * glm::sin(glm::radians(alpha + dalpha));
			// ����������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX + dtexX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));

			alpha += dalpha;
			texX += dtexX;
		}
		texY += dtexY;
		phi += step;
	}

	glGenBuffers(3, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), textcor, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ����������루���1���������Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//���������ݣ����������Ͷ����
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��㷨�������루���2�������������Զ�Ӧ
	glEnableVertexAttribArray(2);  //  // ����Vertex��ɫ�����������2��ȡ�Դ����ݡ�

	glBindVertexArray(0);

	delete[] vertices;
	delete[] textcor;
	delete[] normals;
}

void Curved::SetTexture(GLuint texture)
{
	textureID = texture;
}

void Curved::LoadShader(const char* vsfileName, const char* fsfileName)
{
	prog.CompileShader(vsfileName);
	prog.CompileShader(fsfileName);
	prog.Link();

}

void Curved::Render()
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
	glDrawArrays(GL_TRIANGLES, 0, vertexCount + 20);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//�ر����õ�shader
	prog.Unuse();

}


