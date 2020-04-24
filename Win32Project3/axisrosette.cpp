////////////////////////////////////////////////////////////////////////////////
// Filename: axisrosetteclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "axisrosette.h"


AxisRosette::AxisRosette()
{
	_Triangle = 0;
	_Line = 0;
}

AxisRosette::AxisRosette(const AxisRosette& other)
{
}

AxisRosette::~AxisRosette()
{
	Shutdown();
}

bool AxisRosette::Initialize(D3DClass *direct, float lenght, HWND hwnd)
{
	bool result = true;

	_Line = new VectorLineClass[3]; //0-X,1-Y,2-Z
	if (!_Line) return false;

	_Triangle = new TriangleClass[3];
	if(!_Triangle) return false;

	result &= _Line[0].Initialize(direct, hwnd);
	result &= _Line[1].Initialize(direct, hwnd);
	result &= _Line[2].Initialize(direct, hwnd);

	result &= _Triangle[0].Initialize(direct, hwnd);
	result &= _Triangle[1].Initialize(direct, hwnd);
	result &= _Triangle[2].Initialize(direct, hwnd);

	if (!result) return false;

	_Line[0].SetPoints(2,D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(lenght, 0.0f, 0.0f));
	_Line[0].Color =D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	_Triangle[0].x = lenght;
	_Triangle[0].SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	_Triangle[0].twoWay = true;
	_Triangle[0].rotZ = -D3DX_PI / 2.f;
	_Triangle[0].width = lenght / 10;
	_Triangle[0].height = lenght / 10;

	_Line[1].SetPoints(2,D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, lenght));
	_Line[1].Color=D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	_Triangle[1].z = lenght;
	_Triangle[1].SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	_Triangle[1].twoWay = true;
	_Triangle[1].rotZ = D3DX_PI / 2.f;
	_Triangle[1].rotY = D3DX_PI / 2.f;
	_Triangle[1].width = lenght / 10;
	_Triangle[1].height = lenght / 10;

	_Line[2].SetPoints(2,D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, lenght, 0.0f));
	_Line[2].Color=D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	_Triangle[2].y = lenght;
	_Triangle[2].SetColor(D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
	_Triangle[2].twoWay = true;
	_Triangle[2].width = lenght / 10;
	_Triangle[2].height = lenght / 10;

	return true;
}

bool AxisRosette::Render(D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result = true;
	result &= _Line[0].Render(worldMatrix, baseViewMatrix, orthoMatrix);
	result &= _Line[1].Render(worldMatrix, baseViewMatrix, orthoMatrix);
	result &= _Line[2].Render(worldMatrix, baseViewMatrix, orthoMatrix);

	result &= _Triangle[0].Render(worldMatrix, baseViewMatrix, orthoMatrix);
	result &= _Triangle[1].Render(worldMatrix, baseViewMatrix, orthoMatrix);
	result &= _Triangle[2].Render(worldMatrix, baseViewMatrix, orthoMatrix);
	return result;
}

void AxisRosette::Shutdown()
{
	if (_Line)
	{
		_Line[0].Shutdown();
		_Line[1].Shutdown();
		_Line[2].Shutdown();
		delete[] _Line;
		_Line = 0;
	}
	if (_Triangle) 
	{
		_Triangle[0].Shutdown();
		_Triangle[1].Shutdown();
		_Triangle[2].Shutdown();
		delete[] _Triangle;
		_Triangle = 0;
	}
	return;
}
