#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: VectorLineclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VECTORLINECLASS_H_
#define _VECTORLINECLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "monocolorshader.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: VectorLineClass
////////////////////////////////////////////////////////////////////////////////
class VectorLineClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	};
public:

	bool draw;

public:
	enum DRAW_TYPE {Strip = 3, List = 2};
	VectorLineClass();
	VectorLineClass(const VectorLineClass&);
	~VectorLineClass();

	bool Initialize(D3DClass*, HWND);
	void Shutdown();
	bool Render(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	int GetIndexCount();
	//Set
	bool SetPoint(int, D3DXVECTOR3);
	bool SetPoint(int, float, float, float);
	bool SetPoints(int, D3DXVECTOR3*);
	bool SetPoints(int, D3DXVECTOR3...);
	 
	DRAW_TYPE drawtype;
	D3DXVECTOR4 Color;
private:
	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();
	bool UpdateBuffer();

private:

	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	D3DClass* _Direct3D;
	VertexType* Point_;
	MonocolorShaderClass *Shader_;
	int vertices_count_;
	int* indices_;
	bool isBufferBigEnough;
};
#endif