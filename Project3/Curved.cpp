#include "cgProgram.h" 
#include "Curved.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库
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

	Ka = vec3(0.0, 0.0, 0.3);//环境光
	Kd = vec3(0.0, 0.0, 0.4);//漫反射光
	Ks = vec3(1.0, 1.0, 1.0);//镜面反射光

	shiness = 20;//高光系数

	textureID = 0;
}


Curved::~Curved(void)
{
	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1, &vaoHandle);//删除顶点数组对象
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

	constexpr float step = PI / N; // 曲线步长参数

	const int M = 40;  // 旋转的圆周离散化个数

	vertexCount = N * M * 6;

	float* vertices = new float[vertexCount * 3];
	float* normals = new float[vertexCount * 3];
	float* textcor = new float[vertexCount * 2];
	int verticesIndex = 0;
	int normalsIndex = 0;
	int textorIndex = 0;

	float texX = 0.0f;  // 纹理坐标x
	float texY = 0.0f;  // 纹理坐标y
	float dtexX = 1.0f / M;  // 纹理x增量
	float dtexY = 1.0f / N;  // 纹理y增量

	float alpha = 0;  // 旋转角
	float dalpha = 360.0f / M;  // 角度增量

	float phi = 0.0f;

	for (int i = 0; i < N; i++) {
		Point p1 = curve(phi);
		Point p2 = curve(phi + step);
		// 未旋转的初始法向量1
		float dx1 = 2.0f * glm::sin(phi + glm::pi<float>());
		float dy1 = 1.0f;
		// 法向量1终点坐标
		float nx1 = p1.x + dx1;
		float ny1 = p1.y + dy1;
		// 未旋转的初始法向量2
		float dx2 = 2.0f * glm::sin(phi + step + glm::pi<float>());
		float dy2 = 1.0f;
		// 法向量2终点坐标
		float nx2 = p2.x + dx2;
		float ny2 = p2.y + dy2;
		// 在下方的点的半径
		float r1 = p1.x;
		// 在上方的点的半径
		float r2 = p2.x;
		alpha = 0.0f;
		// 将曲线上各点旋转一周（在此默认转轴为y轴
		for (int j = 0; j < M; j++)
		{
			// 定义两个三角形
			// 第一个三角形
			// 第一个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			// 纹理坐标
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// 第二个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY + dtexY;
			// 法向量
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha)) - r2 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha)) - r2 * glm::sin(glm::radians(alpha));
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX + dtexX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY + dtexY;
			// 法向量
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));
			// 第二个三角形
			// 第一个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY;
			//vertices[verticesIndex++] = texX;
			//vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// 第二个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha + dalpha)) - r1 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha + dalpha)) - r1 * glm::sin(glm::radians(alpha + dalpha));
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			textcor[textorIndex++] = texX + dtexX;
			textcor[textorIndex++] = texY + dtexY;
			//vertices[verticesIndex++] = texX + dtexX;
			//vertices[verticesIndex++] = texY + dtexY;
			// 法向量
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));

			alpha += dalpha;
			texX += dtexX;
		}
		texY += dtexY;
		phi += step;
	}

	glGenBuffers(3, vboHandle);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（纹理）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), textcor, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点纹理输入（序号1，纹理）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);//缓冲区数据（法向量）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader的顶点法向量输入（序号2，法向量）属性对应
	glEnableVertexAttribArray(2);  //  // 允许Vertex着色器中输入变量2读取显存数据。

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
	glDrawArrays(GL_TRIANGLES, 0, vertexCount + 20);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//关闭所用的shader
	prog.Unuse();

}


