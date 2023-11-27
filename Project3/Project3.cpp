// OpenGLOld.cpp : 定义控制台应用程序的入口点。
//
#include "cgProgram.h" //一定要放在glew之前

#include <Windows.h>
#include <math.h>
#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>//OpenGL辅助库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <chrono>
#include <thread>

#include "SOIL/SOIL.h"
//#include "lightPos.h"

#include "cgSphere.h"
#include "cgCube.h"
#include "cgCylinder.h"
#include "Curved.h"

#include "Bitmap.h"


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //这句是不让控制台窗体出现，如果想要出现，去掉即可。


//vec3 viewPos = glm::vec3(2, 2, 5);
//
//vec3 lightcolor = glm::vec3(1, 1, 1);
//
//vec3 Ka = vec3(0.0, 0.0, 0.3);//环境光
//vec3 Kd = vec3(0.0, 0.0, 0.4);//漫反射光
//vec3 Ks = vec3(1.0, 1.0, 1.0);//镜面反射光
//
//int shiness = 20;//高光系数


float headdir[]={0.0f,0.0f,-1.0f};
float moveleftright[] = { 1.0f,0.0f,0.0f };
float rightdir[] = { 0.1f,0.0f,0.0f };

float step = 0.10f;
float beta = 180.0f;//与z轴正向夹角
float betastep = 0.5f;

vec3 pos = vec3(0.0f, 1.0f, 10.0f);
vec3 at;
vec3 viewHead;

cgSphere sphere;
cgCube cube;
cgCylinder cylinder;
Curved curved;

GLfloat light_position[] = { 0.0, 2.0, -1.0, 0.1 };

int rendermode = 0;//0:填充； 1:线框；2：纹理

GLuint textureID;
int  frames=0;

GLuint LoadTexture(char* fname)
{
	GLuint tID;
	
	int width, height, nrComponents;
	unsigned char *data;
	
	data = SOIL_load_image(fname, &width, &height, &nrComponents,SOIL_LOAD_RGB);// 
	
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		bool gammaCorrection = false;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
	
		//生成纹理
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tID);
		glBindTexture(GL_TEXTURE_2D, tID);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		SOIL_free_image_data(data);

		return tID;
	
	}

	return -1;	
}


void LoadShader()
{
	//编译与链接Shader
//	sphere.LoadShader("Shader/3D.vs", "Shader/3D.fs");
	//sphere.LoadShader("Shader/texture.vs", "Shader/texture.fs");
	cube.LoadShader("Shader/tplight.vs", "Shader/tplight.fs");
	cylinder.LoadShader("Shader/tplight.vs", "Shader/tplight.fs");
	curved.LoadShader("Shader/tplight.vs", "Shader/tplight.fs");
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	///////////////////////////
	textureID = LoadTexture("Textures/timg.jpg");

	//地球仪
	//sphere.InitData(1.0f);
	//sphere.SetPos(cgPoint3D(0.0f,1.0f,0.0f));

	//LoadShader();
	//sphere.SetTexture(LoadTexture("Textures/earth2.jpg"));

	cube.InitData(1.0f);
	cube.SetPos(cgPoint3D(0.0f, 1.0f, 0.0f));
	LoadShader();
	cube.SetTexture(LoadTexture("Textures/water1.bmp"));

	cylinder.InitData(1.0f, 5.0f);
	cylinder.SetPos(cgPoint3D(0.0f, 1.0f, 0.0f));
	LoadShader();
	cylinder.SetTexture(LoadTexture("Textures/earth2.jpg"));

	curved.InitData();
	curved.SetPos(cgPoint3D(0.0f, 1.0f, 0.0f));
	LoadShader();
	curved.SetTexture(LoadTexture("Textures/earth2.jpg"));
}

double GetTime() 
{
	using namespace std::chrono;
	auto now = steady_clock::now();
	auto duration = duration_cast<milliseconds>(now.time_since_epoch());
	return duration.count() / 1000.0;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );//清除帧缓存和深度缓存
	
	//模型观察矩阵初始化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i=0; i<3; i++)
		at[i] = pos[i] + headdir[i];

	//sphere.viewMat = glm::lookAt(pos,at,glm::vec3(0.0f, 1.0f, 0.0f));
	cube.viewMat = glm::lookAt(pos, at, glm::vec3(0.0f, 1.0f, 0.0f));
	cylinder.viewMat = glm::lookAt(pos, glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	curved.viewMat = glm::lookAt(pos, glm::vec3(-2.0f,-2.0f,-2.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	if (rendermode==1)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


	//sphere.Render();
	cube.Render();
	cylinder.Render();
	curved.Render();
	
	glutSwapBuffers();//交换缓冲（双缓冲时使用）
}

void change()
{
	float radius = 5.0f;
	float lightX = radius * cos(GetTime());
	float lightZ = radius * sin(GetTime());

	cube.lightPos = glm::vec3(lightX, 2.0f, lightZ);
	cylinder.lightPos = glm::vec3(lightX, 2.0f, lightZ);
	curved.lightPos = glm::vec3(lightX, 2.0f, lightZ);
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);//设置视区（窗口内绘图区域）

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	//sphere.projectionMat = glm::perspective(60.0f,(GLfloat)width/(GLfloat)height, 1.0f, 200.0f);
	cube.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	cylinder.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);
	curved.projectionMat = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':   //上移
		case 'w':
			for (int i=0; i<3; i++)
				pos[i] += step*headdir[i];
			break;
		case 'S':   //下移
		case 's':
			for (int i=0; i<3; i++)
				pos[i] -= step*headdir[i];
			break;
		case 'A':  //左移
		case 'a':
			for (int i = 0; i < 3; i++) {
				pos[i] += step * moveleftright[i];
			}
			break;
		case 'D':   //右移
		case 'd':
			for (int i = 0; i < 3; i++) {
				pos[i] -= step * moveleftright[i];
			}
			break;
		case 'Z':   
		case 'z':
			//抬高相机
	         pos[1] += .30f;
			break;

		case 'X':   
		case 'x':
			//降低相机
	         pos[1] -= .30f;
			break;

		case 'T':   //修改绘制模式
		case 't':
			rendermode = (++rendermode)%2;
			break;	
		case '1':
			light_position[0] += 0.1f;
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
		case '2':
			light_position[0] -= 0.1f;
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			break;
	}

	glutPostRedisplay();
}


void SpecialKey(GLint key,GLint x,GLint y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			for (int i=0; i<3; i++)
				pos[i] += step*headdir[i];
			break;

		case GLUT_KEY_DOWN:
	

			break;

		case GLUT_KEY_LEFT:
			

			break;

		case GLUT_KEY_RIGHT:
			

			break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);//应用程序窗口位置
	glutInitWindowSize(800, 600);//窗口大小
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE| GLUT_DEPTH );//双缓冲，单缓冲为GLUT_SINGLE，深度缓存
	glutCreateWindow("Project Four");//创建窗口，参数为窗口标题

	glewInit();//OpenGL初始化,现代OpenGL需要
	init();

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHT0);

	glutDisplayFunc(display);//图形绘制
	glutReshapeFunc(reshape);//窗口大小变化
	glutKeyboardFunc(keyboard);//键盘交互
	glutSpecialFunc(&SpecialKey);//方向键
	glutIdleFunc(change);

	glutMainLoop();//必须，进入GLUT事件处理循环


	return 0;
}
