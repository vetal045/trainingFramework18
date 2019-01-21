// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Model.h"

GLuint vboId;
Shaders myShaders;

Model model;
Matrix m;
Matrix m2;
Matrix m3;
float m_time;
double size = 0.05;
double rotation = 0.05;
bool left = false, right = false;
double trans = 0.0;

char modelPath[] = "../Resources/Models/Woman1.nfg";

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glEnable(GL_DEPTH_TEST);
	m.SetIdentity();

	model.loadModel(modelPath);
	/*
	//triangle data (heap)
	Vertex verticesData[3];

	verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 0.1f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;

	//генерация буфера в видеопамяти
	glGenBuffers(1, &vboId);
	//привязка сгенерированного буфера (GL_ARRAY_BUFFER указывает на тип буфера)
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//выделение видеопамяти и передача данных
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	//отвязка буфера(одновременно может быть активен только один буфер каждого типа)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);
	//specify shader which we use
	glUseProgram(myShaders.program);

	//connect buffer and specify his type
	glBindBuffer(GL_ARRAY_BUFFER, model.m_hVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.m_hIndexBuffer);

	//передаем матрицу преобразований в шейдер
	glUniformMatrix4fv(myShaders.matrixTransform, 1, false, (GLfloat *)&m);

	GLfloat* ptr = (GLfloat *)0;

	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
		//рассказываем шейдеру где какие атрибуты искать
		//where take data for array of attributes and set format of them
		//1 arg - number of array's attributes
		//2 arg - size of components (3 floats to 1 vertex)
		//3 arg - component type
		//5 arg - Задает смещение байта между последовательными атрибутами вершинной вершины. 
		//Если шаг равен 0, общие атрибуты вершин понимаются плотно упакованными в массив.
		//6 arg - указывает смещение относительно начала вершины	
	}

	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr + 3);
	}

	// отрисовываем, используя вершинный буфер
	glDrawElements(GL_TRIANGLES, model.m_noIndeces, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	m_time += deltaTime;

	if (size > 0 && size < 0.6)
	{
		m.SetScale(size);
	}

	if (rotation > 0.1);
	{
		m2.SetRotationY(rotation);
		m = m*m2;
	}

	//m3.SetTranslation(-0.5f, 0.0f, 0.0f);
	//m = m*m3;

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	switch (key) 
	{
	case 'W':
		size += 0.05;
		break;
	case 'S':
		size -= 0.05;
		break;
	case 'A':
		rotation += 0.05;
		break;
	case 'D':
		rotation -= 0.05;
		break;
	default:
		break;
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &model.m_hVertexBuffer);
	glDeleteBuffers(1, &model.m_hIndexBuffer);
	glDeleteBuffers(1, &vboId);
}

//////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
#ifdef _WIN32

#include <conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	printf("Press any key...\n");
	_getch();

	return 0;
}

#endif
//////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
