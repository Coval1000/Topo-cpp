#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ShapeClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHAPECLASS_H_
#define _SHAPECLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3dclass.h"
#include "monocolorshader.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShapeClass
////////////////////////////////////////////////////////////////////////////////
class ShapeClass
{
protected:
	struct VertexType
	{
		D3DXVECTOR3 position;
	};

	POINT position_;
	D3DXVECTOR4 color_;
	ID3D11Buffer *vertexBuffer_, *indexBuffer_;

	MonocolorShaderClass* Shader_;
	D3DClass* _Direct3D;

public:
	ShapeClass(int = 0, int = 0);
	~ShapeClass();

	const int indicesCount;
	const int verticesCount;

	bool draw;
	double rotX, rotY, rotZ;
	double x, y, z;
	int width, height;
	bool twoWay;

	void Shutdown();
	bool Render(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Initialize(D3DClass*, HWND);
	bool SetColor(float, float, float, float);
	bool SetColor(D3DXVECTOR4);
	bool SetColor(D3DXVECTOR3);
	bool SetColor(D3DXVECTOR3, float);
	bool SetColor(int, int, int, int);
	bool SetColor(int, int, int);
	bool SetColor(D3DXVECTOR3, int);
	//Mutator
protected:
	bool UpdateBuffor();
	bool InitializeBuffor();
	virtual void CreateVertices(VertexType*) = 0;
	virtual void CreateIndices(unsigned long*) = 0;
};

#endif