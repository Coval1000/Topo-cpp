#pragma once

#ifndef _PLAIN_H_
#define _PLAIN_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include "D3dclass.h"
#include <string>
#include <vector>
class Plain 
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
public:
	Plain();
	Plain(const Plain&);
	~Plain();

	enum ColorMode
	{
		GreyScale, HighContrast, HCGSHybrid, FullBit, SolidColor, Undefined, FullBitEyeCorrect
	};

	enum DataMode
	{
		Extrapolator0, Extrapolator1, Extrapolator1Interpolated, Raw
	};

	enum RenderStyle
	{
		Solid, Grid
	};

	bool Initialize(D3DClass*, D3DXVECTOR3*, int, int, float, float, RenderStyle, ColorMode = GreyScale);

	void Render();

	int GetIndexCount();
	int GetTerrainWidth();
	int GetTerrainHeight();
	int GetGridDensity();
	int GetOffSetX();
	int GetOffSetY();
	float GetTerrainValue(int, int);

	bool ChangeColorMode(ColorMode);
	bool ChangeGridSize(int);
	bool SetMode(DataMode);
	void Shutdown();
private:
	HRESULT FillVertex();
	HRESULT FillIndex(unsigned long*);
	HRESULT FillIndexGrid(unsigned long*);
	bool InitializePlot(unsigned long**);
	void InitializeVertices();
	void InitializeIndices(unsigned long*);
	void IndexRaw(unsigned long*);
	void IndexGridRaw(unsigned long*);
	void IndexInterpolated0(unsigned long*);
	void IndexGridInterpolated0(unsigned long*);
	void IndexInterpolated1(unsigned long*);
	void IndexGridInterpolated1(unsigned long*);
	bool Update(bool, bool, bool);
	bool UpdateTerrain(VertexType*);
	bool UpdateColor(VertexType*);
	bool InitializeBuffers();
	void ShutdownBuffers();
	D3DXVECTOR4 HighContrastBuffer(double);
	D3DXVECTOR4 HybrydBuffer(double);
	D3DXVECTOR4 GreyScaleBuffer(double);
	D3DXVECTOR4	SolidBuffer(double);
	D3DXVECTOR4 FullBitBuffer(double);
	D3DXVECTOR4 FullBitEyeCorrectBuffer(double);
private:
	int _terrainWidth, _terrainHeight;
	int _offSetX, _offSetY;
	int _vertexCount, _indexCount;
	int griddensity_;
	DataMode PLOTTING;
	RenderStyle RENDERING;
	double _top, _bottom;
	ID3D11Buffer* _vertexBuffer, * _indexBuffer;
	D3DXVECTOR3* HeightMap_;
	ColorMode COLOR_MODE_;
	VertexType* Vertices_;
	D3DClass* D3Dx_;
};

#endif