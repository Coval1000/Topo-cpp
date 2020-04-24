#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BUTTON_H_
#define _BUTTON_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"
#include "textureclass.h"
#include "D3dclass.h"
#include "colorshaderclass.h"
#include "RectangleClass.h"
class Button;
#include "applicationclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Button
////////////////////////////////////////////////////////////////////////////////

class Button
{
public:
	D3DXVECTOR4 textColor;
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		D3DXVECTOR4 color_secondary;
	};

	struct VertexTextType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct FontType
	{
		float left, right;
		int size;
	};

	POINT position_;
	int _width, _height;
	char* _text;
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	ID3D11Buffer *_vertexTextBuffer, *_indexTextBuffer;
	int _vertexTextCount, _indexTextCount, _maxLength, _Length;
	int _vertexCount, _indexCount;
	int screenHeight_, screenWidth_;
	VertexType* _vertices;
	VertexTextType* _verticesText;
	FontType *Font_;
	TextureClass* _Texture;
	FontShaderClass* FontShader_;

	D3DClass* _Direct;
	RectangleClass *_Rectangle;

public:
	Button(ApplicationClass*);
	~Button();

	bool draw;

	bool Initialize(D3DClass*, HWND);

	bool Render(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Hover(int, int);
	bool Click(int, int);
	void Shutdown();
	//#######
	//Mutator
	//#######
	void SetText(char*);
	void SetPosition(int, int);
	void SetRectangleColor(D3DXVECTOR4);
	void SetSentence();
	void SetRectangle(int_RECT);
	void OnClick(On_Click*);
private:
	bool UpdateSentence();

	bool InitializeSentence();
	bool LoadFontData(char*);
	bool LoadTexture(WCHAR*);
	void ShutdownBuffers();

	void RenderSentence();
	void RenderRectangle();
	On_Click _Click;
	ApplicationClass* _App;
};

#endif

