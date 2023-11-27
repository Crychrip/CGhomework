#include "Bezier.h"

Bezier::Bezier()
{
	ProjectionMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::mat4(1.0f);
	vao = vbo = -1;
	TextureID = ProgramID = 0;
	vertexCount = 0;
}

Bezier::Bezier(std::vector<Point>& points)
{
	Points = points;
	createData();
}

Bezier::~Bezier()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Bezier::SetPoints(std::vector<Point>& points)
{
	Points = points;
}

void Bezier::Render()
{
	glUseProgram(ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glUniform1i(glGetUniformLocation(ProgramID, "Tex"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount + 20);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Bezier::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Bezier::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Bezier::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Bezier::SetProgram(unsigned int programID)
{
	ProgramID = programID;
}

void Bezier::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}


// 计算组合数
int binomial(int n, int i) {
	int res = 1;
	for (int j = 1; j <= i; ++j) {
		res *= (n - j + 1) / (double)j; //(double)十分关键，不然j=i=n时，j为分数=0；
	}
	return res;
}
// 计算n次贝塞尔曲线上的点
Point Bezier::bezier_curve(float t) {
	int n = Points.size() - 1;
	Point res;
	res.x = res.y = res.z = 0;
	for (int i = 0; i <= n; ++i) {
		float b = binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
		res.x = res.x + Points[i].x * b;
		res.y = res.y + Points[i].y * b;
		res.z = res.z + Points[i].z * b;
	}
	return res;
}

void Bezier::createData()
{
	const int N = 31;

	const float step = 1.0f / (N - 1); // 贝塞尔曲线步长参数

	const int M = 40;  // 旋转的圆周离散化个数

	vertexCount = N * M * 6;

	float* vertices = new float[vertexCount * 5];
	int verticesIndex = 0;

	float texX = 0.0f;  // 纹理坐标x
	float texY = 0.0f;  // 纹理坐标y
	float dtexX = 1.0f / M;  // 纹理x增量
	float dtexY = step;  // 纹理y增量

	float alpha = 0;  // 旋转角
	float dalpha = 360.0f / M;  // 角度增量

	for (float t = 0; t <= 1; t += step) {
		Point p1 = bezier_curve(t);
		Point p2 = bezier_curve(t + step);
		// 在下方的点的半径
		float r1 = glm::sqrt(glm::pow(p1.x, 2.0f) + glm::pow(p1.z, 2.0f));
		// 在上方的点的半径
		float r2 = glm::sqrt(glm::pow(p2.x, 2.0f) + glm::pow(p2.z, 2.0f));

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
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// 第二个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY + dtexY;
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;

			// 第二个三角形
			// 第一个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// 第二个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY;
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;

			alpha += dalpha;
			texX += dtexX;
		}
		texY += dtexY;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 5 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}