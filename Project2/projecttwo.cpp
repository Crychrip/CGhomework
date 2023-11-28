// ProjectOne.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "cgProgram.h" //һ��Ҫ����glew֮ǰ

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using glm::vec4;

cgProgram prog;

// ���妰
#define PI 3.14159265

unsigned int vboHandle[2];
unsigned int vaoHandle;

unsigned int vboHandle2[2];
unsigned int vaoHandle2;

unsigned int vboHandle3[2];
unsigned int vaoHandle3;

mat4 projection;
mat4 model1;
mat4 model2;
mat4 model;

// ��������ṹ��
struct Vertex {
	float x;
	float y;
};

// ��ȡ��Բ����ǵ��嶥��(��������ԲԲ�������Լ��뾶)
Vertex* getExternalVertex(Vertex Center_Vertex, double R) {
	// ����Vertex�ṹ�����飬������������һ��Բ��
	Vertex* externalVertex = new Vertex[6];
	float alpha = 2 * PI / 5;
	float thet = PI / 10;
	float bet = 2 * PI / 10;
	// Բ������
	externalVertex[0].x = Center_Vertex.x, externalVertex[0].y = Center_Vertex.y;
	// �����������
	externalVertex[1].x = Center_Vertex.x, externalVertex[1].y = Center_Vertex.y + R;
	externalVertex[2].x = Center_Vertex.x + R * cos(thet), externalVertex[2].y = Center_Vertex.y + R * sin(thet);
	externalVertex[3].x = Center_Vertex.x + R * sin(bet), externalVertex[3].y = Center_Vertex.y - R * cos(bet);
	externalVertex[4].x = Center_Vertex.x - R * sin(bet), externalVertex[4].y = Center_Vertex.y - R * cos(bet);
	externalVertex[5].x = Center_Vertex.x - R * cos(thet), externalVertex[5].y = Center_Vertex.y + R * sin(thet);

	return externalVertex;
}

// ��ȡ��Բ����ǵ��嶥��(������СԲԲ�������Լ��뾶)
Vertex* getInternalVertex(Vertex Center_Vertex, double r) {
	// ����Vertex�ṹ�����飬������������һ��Բ��
	Vertex* internalVertex = new Vertex[6];
	float alpha = 2 * PI / 5;
	float thet = PI / 10;
	float bet = (PI - alpha) / 2;
	// Բ������
	internalVertex[0].x = Center_Vertex.x, internalVertex[0].y = Center_Vertex.y;
	// �����������
	internalVertex[1].x = Center_Vertex.x + r * cos(bet), internalVertex[1].y = Center_Vertex.y + r * sin(bet);
	internalVertex[2].x = Center_Vertex.x + r * cos(thet), internalVertex[2].y = Center_Vertex.y - r * sin(thet);//
	internalVertex[3].x = Center_Vertex.x, internalVertex[3].y = Center_Vertex.y - r;//
	internalVertex[4].x = Center_Vertex.x - r * cos(thet), internalVertex[4].y = Center_Vertex.y - r * sin(thet);
	internalVertex[5].x = Center_Vertex.x - r * cos(bet), internalVertex[5].y = Center_Vertex.y + r * sin(bet);

	return internalVertex;
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//���ñ���ɫ
}

void LoadShader()
{

	//����������Shader
	prog.CompileShader("Shader/basic.vs");
	prog.CompileShader("Shader/basic.fs");
	prog.Link();
}

float a = 500.0f, b = 500.0f;//������  (100,100) 500*500;
float leftbottomx = 100.0f; float leftbottomy = 100.0f;

float starR = 100.0f; float starr = starR / 10.0 * 4.0;//���������
float star1x = 300.0f, star1y = 300.0f;//��һ�����������
float star2x = 200.0f, star2y = 200.0f;//�ڶ������������

//��һ������ǽ�����ײ������x��y������ٶȣ�������ײ�߽�  ������ת��������ת�Ǻ�����
float star1nowx = star1x, star1nowy = star1y;
float star1speedx = 0.5f, star1sppedy = 0.1f;
float star1angle = 0;  // ��ת��
float star1alpha = 10;  // ��ת����

//�ڶ��������ͬ��һ������ǣ�����Ҫ����
float star2nowx = star2x, star2nowy = star2y;
float star2speedx = -0.5f, star2sppedy = -0.1f;
float star2angle = 0;  // ��ת��
float star2alpha = -10;  // ��ת����
float star2scale = 1;  // ������
float star2ds = 0.0005;  // ������

float left = leftbottomx, right = leftbottomx + b, bottom = leftbottomy, top = leftbottomy + a;//�߽�

void drawrectangle(void)
{
	float * vertc = new float[4 * 2];
	float * color = new float[4 * 3];
	int countv = 0;
	int countc = 0;

	vertc[countv++] = leftbottomx; vertc[countv++] = leftbottomy;
	vertc[countv++] = leftbottomx; vertc[countv++] = leftbottomy + a;
	vertc[countv++] = leftbottomx + b; vertc[countv++] = leftbottomy + a;
	vertc[countv++] = leftbottomx + b; vertc[countv++] = leftbottomy;

	for (int i = 1; i <= 4; i++)
	{
		color[countc++] = 1.0f;
		color[countc++] = 0.0f;
		color[countc++] = 0.0f;
	}

	glGenBuffers(2, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertc, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ���ɫ���Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), color, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader�Ķ�����ɫ���루���1����ɫ�����Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindVertexArray(0);

	delete[] vertc;
	delete[] color;
}

void fillstarone(void)
{
	Vertex Center_Vertex;
	Center_Vertex.x = star1x, Center_Vertex.y = star1y;

	Vertex* externalVertex = getExternalVertex(Center_Vertex, starR);
	Vertex* internalVertex = getInternalVertex(Center_Vertex, starr);

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

	glGenBuffers(2, vboHandle2);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle2);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle2);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle2[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 30 * 2 * sizeof(float), vertc2, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle2[1]);//���������ݣ���ɫ���Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 30 * 3 * sizeof(float), color2, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ�����ɫ���루���1����ɫ�����Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc2;
	delete[] color2;
}


void fillstartwo(void)
{
	Vertex Center_Vertex;
	Center_Vertex.x = star2x, Center_Vertex.y = star2y;

	Vertex* externalVertex = getExternalVertex(Center_Vertex, starR);
	Vertex* internalVertex = getInternalVertex(Center_Vertex, starr);

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

	glGenBuffers(2, vboHandle3);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle3);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle3);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle3[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 30 * 2 * sizeof(float), vertc3, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle3[1]);//���������ݣ���ɫ���Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 30 * 3 * sizeof(float), color3, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));//Vertex Shader�Ķ�����ɫ���루���1����ɫ�����Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertc3;
	delete[] color3;
}

void reshape(int width, int height)//���ڴ�С�仯ʱִ��
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void change()
{
	model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(star1nowx, star1nowy, 0.0));
	model1 = glm::rotate(model1, glm::radians(star1angle), glm::vec3(0.0, 0.0, 1.0));
	model1 = glm::translate(model1, glm::vec3(-star1x, -star1y, 0.0));

	model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(star2nowx, star2nowy, 0.0f));
	model2 = glm::rotate(model2, glm::radians(star2angle), glm::vec3(0.0f, 0.0f, 1.0f));
	model2 = glm::scale(model2, glm::vec3(star2scale, star2scale, star2scale));
	model2 = glm::translate(model2, glm::vec3(-star2x, -star2y, 0.0f));

	if (star1nowx + starR >= right) {
		star1speedx = -star1speedx;
		star1nowx = right - starR;
	}
	else if (star1nowx - starR <= left) {
		star1speedx = -star1speedx;
		star1nowx = left + starR;
	}
	if (star1nowy + starR >= top) {
		star1sppedy = -star1sppedy;
		star1nowy = top - starR;
	}
	else if (star1nowy - starR <= bottom) {
		star1sppedy = -star1sppedy;
		star1nowy = bottom + starR;
	}

	if (star2nowx + starR * star2scale>= right) {
		star2speedx = -star2speedx;
		star2nowx = right - starR * star2scale;
	}
	else if (star2nowx - starR * star2scale <= left) {
		star2speedx = -star2speedx;
		star2nowx = left + starR * star2scale;
	}
	if (star2nowy + starR * star2scale >= top) {
		star2sppedy = -star2sppedy;
		star2nowy = top - starR * star2scale;
	}
	else if (star2nowy - starR * star2scale <= bottom) {
		star2sppedy = -star2sppedy;
		star2nowy = bottom + starR * star2scale;
	}

	float d = sqrt(pow(star1nowx - star2nowx, 2) + pow(star1nowy - star2nowy, 2));
	if (d <= starR + starR * star2scale
		) {
		d = (starR + starR * star2scale - d) / 2 * abs(star1nowx - star2nowx) / d;
		star1nowx += glm::sign(star1nowx - star2nowx) * d;
		star2nowx += glm::sign(star2nowx - star1nowx) * d;
		star1nowy += glm::sign(star1nowy - star2nowy) * d;
		star2nowy += glm::sign(star2nowy - star1nowy) * d;
		// ��ȫ������ײ
		/*��ײǰ������С��Ķ����ֱ�Ϊ��
			p1 = m1 * v1 = m1 * (v1x, v1y)
			p2 = m2 * v2 = m2 * (v2x, v2y)
			���У�m1 �� m2 �ֱ�������С���������
			������ײ����ȫ���Եģ������ײ������С��Ķ�����Ȼ�غ㣬��
			p1' + p2' = p1 + p2
			���У�p1' �� p2' �ֱ�����ײ������С��Ķ�����
			ͬʱ��������ײǰ������С��֮�������ٶȷ����෴�������ײ������С����ٶȴ�С���䣬���򽻻���
			��ˣ���ײ������С����ٶȷֱ�Ϊ��
			v1' = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2)
			v2' = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2)
		*/

		//�������ǵ��ܶ�Ϊ1����������2�����ţ�����2��������ϱ仯�����ǵ�����ӦΪ4/3*PI*R^3
		float m1 = 4.0 / 3.0 * PI * pow(starR, 3);
		float m2 = 4.0 / 3.0 * PI * pow(starR * star2scale, 3);
		float vx1 = star1speedx;
		float vy1 = star1sppedy;
		float vx2 = star2speedx;
		float vy2 = star2sppedy;

		star1speedx = ((m1 - m2) * vx1 + 2 * m2 * vx2) / (m1 + m2);
		star1sppedy = ((m1 - m2) * vy1 + 2 * m2 * vy2) / (m1 + m2);
		star2speedx = ((m2 - m1) * vx2 + 2 * m1 * vx1) / (m1 + m2);
		star2sppedy = ((m2 - m1) * vy2 + 2 * m1 * vy1) / (m1 + m2);
	}

	star1nowx += star1speedx;
	star1nowy += star1sppedy;
	star1angle += star1alpha;

	star2nowx += star2speedx;
	star2nowy += star2sppedy;
	star2angle += star2alpha;
	star2scale += star2ds;
	if (star2scale >= 2) {
		star2ds = -star2ds;
		star2scale = 2;
	}
	else if (star2scale <= 0.1) {
		star2scale = 0.1;
		star2ds = -star2ds;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)//��Ӧ����
{
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//������Ҫ��shader�����Զ�̬�л���ͬ��shader
	prog.Use();

	prog.SetUniform("ProjectionMatrix", projection);

	prog.SetUniform("ModelMatrix", model);
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_LINE_LOOP, 0, 4);//GL_LINES�߶�
	glBindVertexArray(0);

	prog.SetUniform("ModelMatrix", model1);
	glBindVertexArray(vaoHandle2);
	glDrawArrays(GL_TRIANGLES, 0, 30);
	glBindVertexArray(0);

	prog.SetUniform("ModelMatrix", model2);
	glBindVertexArray(vaoHandle3);
	glDrawArrays(GL_TRIANGLES, 0, 30);//GL_LINES�߶�
	glBindVertexArray(0);
	//�ر����õ�shader
	prog.Unuse();
	//glFlush();//������ʱ����Ҫ��˵����ͼ�������������
	glutSwapBuffers();//�������壨˫����ʱʹ�ã�
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//˫���壬������ΪGLUT_SINGLE
	glutCreateWindow("Project Two");

	glewInit();//OpenGL��ʼ��,�ִ�OpenGL��Ҫ
	init();

	LoadShader();//����Shader

	drawrectangle();
	fillstarone();
	fillstartwo();

	glutDisplayFunc(display);//ͼ�λ���
	glutReshapeFunc(reshape);//���ڴ�С�仯
	glutKeyboardFunc(keyboard);//���̽���
	glutIdleFunc(change);

	glutMainLoop();

	glDeleteBuffers(2, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1, &vaoHandle);//ɾ�������������

	return 0;
}