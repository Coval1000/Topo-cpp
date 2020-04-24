#pragma once
#pragma once
#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: axisrosette.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _AXLE_H_
#define _AXLE_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "TriangleClass.h"
#include "VectorLineClass.h"
#include "textclass.h"

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: AxleClass
////////////////////////////////////////////////////////////////////////////////
class Axle
{

public:
	enum DIVISION_TYPE {Const_Value, Const_Amount};
	Axle();
	Axle(const Axle&);
	~Axle();

	bool Initialize(D3DClass*, HWND);
	void Shutdown();
	bool Render(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool RenderBanerLabel(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,D3DXVECTOR3);

	void SetValuesRange(float, float, std::string);
	void SetLenght(float);
	void SetPosition(D3DXVECTOR3);
	void SetUnit(std::string);
	void SetTextSize(float);
	void SetTextOffset(D3DXVECTOR3);
	void SetSupportDivision(float);
	void SetSupportDivisionLenght(float);
	void SetMainDivision(float);
	void SetMainDivisionLenght(float);
	void SetDivisionType(DIVISION_TYPE);
	void SetDirection(D3DXVECTOR3);
	bool Validate();
	bool IsValidated();
private:
	bool DivideCosntAmount();
	bool DivideCosntValue();

	D3DXVECTOR3 axlerotation_;
	D3DXVECTOR3 axlebeginposition_;
	D3DXVECTOR3 textposition_;
	D3DXVECTOR3 textOffset_;
	Text *Label_;
	VectorLineClass *Axle_;
	std::string unit_;
	float axleLenght_;
	float maxValue_;
	float minValue_;
	float supportDivision_;
	float supportDivisionLenght_;
	float mainDivision_;
	float mainDivisionLenght_;
	DIVISION_TYPE division_type_;
	bool validated_;
};

#endif