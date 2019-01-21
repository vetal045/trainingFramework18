#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"

class Model
{
public:
	Model();

	int loadModel(const char * pathModel);

	GLuint m_hVertexBuffer, m_hIndexBuffer;
	Vertex *m_pVerticesData;
	unsigned int * m_pIndices;
	unsigned int m_noVertices, m_noIndeces;
};