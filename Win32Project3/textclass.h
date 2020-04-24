#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXT_H_
#define _TEXT_H_

#include <string>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"
#include "D3dclass.h"

constexpr auto _default_length = 32;
////////////////////////////////////////////////////////////////////////////////
// Class name: Text
////////////////////////////////////////////////////////////////////////////////
class Text
{
private:
	
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(D3DClass*, HWND);
	bool Initialize(D3DClass*, HWND, string**, string**);
	void Shutdown();
	bool Render(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool SetText(std::string);
	bool SetText(char*);

private:
	bool InitializeSentence(int);
	bool UpdateSentence(char*);
	bool UpdateSentence(std::string);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
public:
	D3DXVECTOR4 color;
	D3DXVECTOR3 position;
	float scale;

private:
	FontClass* Font_;
	FontShaderClass* FontShader_;
	D3DClass* D3dx_;

	SentenceType* sentence_;
};

#endif

