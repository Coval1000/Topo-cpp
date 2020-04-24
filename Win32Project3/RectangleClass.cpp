///////////////////////////////////////////////////////////////////////////////
// Filename: RectangleClass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "RectangleClass.h"

void RectangleClass::CreateVertices(VertexType *vertices)
{
	vertices[0].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f); //tl
	vertices[1].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f); //tp
	vertices[2].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f); //bl
	vertices[3].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f); //br
}

void RectangleClass::CreateIndices(unsigned long* indices)
{
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	indices[6] = 0;
	indices[7] = 2;
	indices[8] = 1;

	indices[9] = 3;
	indices[10] = 1;
	indices[11] = 2;
}