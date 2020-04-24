#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "D3dclass.h"
#include "Plain.h"
#ifndef _COCLASS_AGENT_
#define _COCLASS_AGENT_
#include "C:\\Users\\User\\Documents\\Visual Studio 2015\\Projects\\ComServerWP3\\ComServerWP3\\ComServerWP3TypeInfo_h.h"
#include "C:\\Users\\User\\Documents\\Visual Studio 2015\\Projects\\ComServerWP3\\ComServerWP3\\ComServerWP3TypeInfo_i.c"
#endif // !_COCLASS_AGENT_

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		D3DXVECTOR4 color_secondary;
	};

public:
	enum DataMode
	{
		Extrapolator0, Extrapolator1, Extrapolator1Interpolated, Raw
	};

	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();
	bool Initialize(D3DClass*, string**, string**, LPWSTR, Plain::ColorMode = Plain::GreyScale);
	bool Initialize(D3DClass*, int, int, D3DXVECTOR3* , Plain::ColorMode = Plain::GreyScale);
	void Shutdown();

	float GetTop();
	float GetBottom();

	bool HighlightNode(POINT);
	bool LoadComMap();


	Plain Plot;
	Plain Grid;
private:
	bool LoadHeightMap(string**, string**, LPWSTR);
	void ShutdownHeightMap();

private:
	int terrainWidth_, terrainHeight_;
	double _top, _bottom;
	D3DXVECTOR3* HeightMap_;
	POINT ClickPoint_;
	D3DClass *D3Dx_;

};

inline bool operator==(const POINT&, const POINT&);
inline bool operator!=(const POINT&, const POINT&);
#endif

