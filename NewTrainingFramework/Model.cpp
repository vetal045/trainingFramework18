#include "stdafx.h"
#include "Model.h"
#include <iostream>

Model::Model()
{
	m_hIndexBuffer = 0;
	m_hVertexBuffer = 0;
}

int Model::loadModel(const char * pathModel)
{
	FILE * file = fopen(pathModel, "r");

	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return 1;
	}

	fscanf_s(file, "NrVertices: %d", &m_noVertices);

	std::cout << "NrVertices: " << m_noVertices << "\n"; //Show result about number of vertices

	m_pVerticesData = new Vertex[m_noVertices];

	for (int i = 0; i < m_noVertices; i++)
	{
		fscanf_s(file, " %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%*f, %*f];", &m_pVerticesData[i].pos.x, &m_pVerticesData[i].pos.y, &m_pVerticesData[i].pos.z);
	}

	
	//for (auto i = 0; i < m_noVertices; i++) //Show positions of all vertices
	//{
	//	m_pVerticesData[i].pos.y += -0.9;
	//}
	
	for (auto i = 0; i < m_noVertices; i++) //Show positions of all vertices
	{
		std::cout << m_pVerticesData[i].pos.x << " " << m_pVerticesData[i].pos.y << " " << m_pVerticesData[i].pos.z << std::endl;
	}

	fscanf_s(file, " ");
	fscanf_s(file, "NrIndices: %d", &m_noIndeces);

	std::cout << "NrIndices: " << m_noIndeces << "\n"; //Show result about number of indices

	m_pIndices = new unsigned int[m_noIndeces];

	for (int i = 0; i < m_noIndeces; i += 3)
	{
		fscanf_s(file, "   %*d.    %d,    %d,    %d", &m_pIndices[i], &m_pIndices[i + 1], &m_pIndices[i + 2]);
	}

	{
		//Show all indeces

		int count = 0;
		for (auto i = 0; i < m_noIndeces; i++)
		{
			std::cout << m_pIndices[i] << " ";
			++count;

			if (count == 3)
			{
				std::cout << std::endl;
				count = 0;
			}
		}
	}

	glGenBuffers(1, &m_hIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_noIndeces, m_pIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);

	glGenBuffers(1, &m_hVertexBuffer); //buffer object name generation
	glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer); //buffer object binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_noVertices, m_pVerticesData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);

	fclose(file);

	delete[] m_pVerticesData;
	//m_pVerticesData = nullptr;

	delete[] m_pIndices;
	//m_pIndices = nullptr;
}
