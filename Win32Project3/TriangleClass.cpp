///////////////////////////////////////////////////////////////////////////////
// Filename: TriangleClass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "TriangleClass.h"

void TriangleClass::CreateIndices(unsigned long* indices)
{
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 0;
}

void TriangleClass::CreateVertices(VertexType* vertices)
{
	vertices[0].position = D3DXVECTOR3(0.0f, 0.5f, 0.0f); //t
	vertices[1].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f); //r
	vertices[2].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f); //l
}