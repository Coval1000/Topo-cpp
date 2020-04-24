///////////////////////////////////////////////////////////////////////////////
// Filename: buttonclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "button.h"

Button::Button(ApplicationClass* app)
{
	_App = app;
	_App->Screen(screenWidth_, screenHeight_);
	textColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	_vertexCount = 4;
	_indexCount = 6;

	_vertices = nullptr;
	_text = " ";
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
	_vertexTextBuffer = nullptr;
	_indexTextBuffer = nullptr;
	_verticesText = nullptr;
	Font_ = nullptr;
	_Texture = nullptr;
	FontShader_ = nullptr;
	_Direct = nullptr;
	_maxLength = 256;
	_Length = 0;
	draw = true;
	_Rectangle = nullptr;
	_Click = nullptr;
}

Button::~Button()
{
	Shutdown();
}

bool Button::Initialize(D3DClass* direct, HWND hwnd)
{
	bool result;
	_Direct = direct;

	_Rectangle = new RectangleClass;
	if (!_Rectangle) return false;
	_Rectangle->Initialize(direct, hwnd);

	FontShader_ = new FontShaderClass;
	if (!FontShader_)
	{
		return false;
	}

	result = FontShader_->Initialize(direct->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	result = LoadFontData("../Win32Project3/data/fontdata.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not load the font data.", L"Error", MB_OK);
		return false;
	}

	result = LoadTexture(L"../Win32Project3/data/font.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture object.", L"Error", MB_OK);
		return false;
	}

	_Click = _App->fPTR;

	return true;
}

bool Button::InitializeSentence()
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	ID3D11Device* device = _Direct->GetDevice();
	_Length = (int)strlen(_text);

	_vertexTextCount = 6 * _maxLength;

	// Set the number of indexes in the index array.
	_indexTextCount = _vertexTextCount;

	_verticesText = new VertexTextType[_vertexTextCount];
	if (!_verticesText)
	{
		return false;
	}

	indices = new unsigned long[_indexTextCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(_verticesText, 0, (sizeof(VertexTextType) * _indexTextCount));

	SetSentence();

	// Initialize the index array.
	for (i = 0; i<_indexTextCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextType) * _vertexTextCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = _verticesText;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexTextBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexTextCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexTextBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

void Button::SetSentence()
{
	float drawX, drawY;
	int lenght = 0;
	int index, i, letter;

	index = 0;
	for (i = 0; i < _Length; i++)
	{
		letter = ((int)_text[i]) - 32;

		if (letter == 0)
		{
			lenght = lenght + 3;
		}
		else
		{
			lenght = lenght + Font_[letter].size + 1;
		}
	}

	drawX = (float)(_Rectangle->x - lenght / 2.f);
	drawY = (float)(_Rectangle->y);

	for (i = 0; i < _Length; i++)
	{
		letter = ((int)_text[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			_verticesText[index].position = D3DXVECTOR3(drawX, drawY + 6, 0.0f);  // Top left.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			_verticesText[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 10), 0.0f);  // Bottom right.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			_verticesText[index].position = D3DXVECTOR3(drawX, (drawY - 10), 0.0f);  // Bottom left.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			_verticesText[index].position = D3DXVECTOR3(drawX, drawY + 6, 0.0f);  // Top left.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			_verticesText[index].position = D3DXVECTOR3(drawX + Font_[letter].size, drawY + 6, 0.0f);  // Top right.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].right, 0.0f);
			index++;

			_verticesText[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 10), 0.0f);  // Bottom right.
			_verticesText[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + Font_[letter].size + 1.0f;
		}
	}
	return;
}

bool Button::UpdateSentence()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ID3D11DeviceContext * deviceContext;
	deviceContext = _Direct->GetDeviceContext();

	_Length = (int)strlen(_text);

	_vertexTextCount = 6 * _maxLength;

	// Set the number of indexes in the index array.
	_indexTextCount = _vertexTextCount;


	{
		unsigned long* indices;
		indices = new unsigned long[_indexTextCount];
		if (!indices)
		{
			return false;
		}

		for (int i = 0; i < _indexTextCount; i++)
		{
			indices[i] = i;
		}

		result = deviceContext->Map(_indexTextBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			delete[] indices;
			indices = 0;
			return false;
		}

		// Copy the data into the vertex buffer.
		memcpy(mappedResource.pData, (void*)indices, (sizeof(unsigned long) * _indexTextCount));

		// Unlock the vertex buffer.
		deviceContext->Unmap(_indexTextBuffer, 0);
		delete[] indices;
		indices = 0;
	}

	{
		delete[] _verticesText;

		_verticesText = new VertexTextType[_vertexTextCount];
		if (!_verticesText)
		{
			return false;
		}
		// Initialize vertex array to zeros at first.
		memset(_verticesText, 0, (sizeof(VertexTextType) * _vertexTextCount));

		SetSentence();

		// Lock the vertex buffer so it can be written to.
		result = deviceContext->Map(_vertexTextBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		// Copy the data into the vertex buffer.
		memcpy(mappedResource.pData, (void*)_verticesText, (sizeof(VertexTextType) * _vertexTextCount));

		// Unlock the vertex buffer.
		deviceContext->Unmap(_vertexTextBuffer, 0);
	}

	return true;
}

//######
//Render
//######
bool Button::Render(D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix,
	D3DXMATRIX orthoMatrix)
{
	if (!draw) return true;
	bool result;
	RenderRectangle();
	result = _Rectangle->Render(worldMatrix, baseViewMatrix, orthoMatrix);

	RenderSentence();
	result &= FontShader_->Render(_Direct->GetDeviceContext(), _indexTextCount, worldMatrix, baseViewMatrix, orthoMatrix, _Texture->GetTexture(),
		textColor);

	return result;
}

void Button::RenderSentence()
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	//bool result;
	ID3D11DeviceContext* deviceContext = _Direct->GetDeviceContext();
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTextType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexTextBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexTextBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Button::RenderRectangle()
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	ID3D11DeviceContext* deviceContext = _Direct->GetDeviceContext();
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Button::Hover(int x, int y)
{
	if (x >= _Rectangle->x &&
		x <= _Rectangle->x + _width &&
		y >= _Rectangle->y &&
		y <= _Rectangle->y + _height) return true;
	else return false;

}

bool Button::Click(int x, int y)
{
	if (x >= position_.x &&
		x <= position_.x + _width &&
		y >= position_.y &&
		y <= position_.y + _height)
	{
		if (_Click)
		{
			(*_App.*_Click)();
		}
		return true;
	}
	else return false;
}
//########
//Mutators
//########
void Button::SetText(char* text)
{
	_text = text;
	if(_Length) UpdateSentence();
	else InitializeSentence();
	return;
}

void Button::SetPosition(int x, int y)
{
	_Rectangle->x = x - (screenWidth_ - _width) / 2;
	_Rectangle->y = -y + (screenHeight_ - _height) / 2;
	position_.x = x;
	position_.y = y;
	SetText(_text);
	return;
}

void Button::SetRectangleColor(D3DXVECTOR4 color)
{
	_Rectangle->SetColor(color);
	return;
}

void Button::SetRectangle(int_RECT rect)
{
	_width = rect.width;
	_height = rect.height;
	_Rectangle->height = _height;
	_Rectangle->width = _width;
	SetPosition(rect.left, rect.top);
	return;
}

void  Button::OnClick(On_Click *e)
{
	_Click = *e;
	return;
}

//
//
//

bool Button::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	Font_ = new FontType[95];
	if (!Font_)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ' &&  temp != '\t')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ' &&  temp != '\t')
		{
			fin.get(temp);
		}

		fin >> Font_[i].left;
		fin >> Font_[i].right;
		fin >> Font_[i].size;
	}

	fin.close();

	return true;
}

bool Button::LoadTexture(WCHAR* filename)
{
	bool result;


	// Create the texture object.
	_Texture = new TextureClass;
	if (!_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = _Texture->Initialize(_Direct->GetDevice(), filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Button::Shutdown()
{
	if (_vertices)
	{
		delete[] _vertices;
		_vertices = 0;
	}

	if (_verticesText)
	{
		delete[] _verticesText;
		_verticesText = 0;
	}

	if (Font_)
	{
		delete[] Font_;
		Font_ = 0;
	}

	if (_Texture)
	{
		_Texture->Shutdown();
		delete _Texture;
		_Texture = 0;
	}

	if (FontShader_)
	{
		FontShader_->Shutdown();
		delete FontShader_;
		FontShader_ = 0;
	}

	if (_Rectangle)
	{
		_Rectangle->Shutdown();
		delete _Rectangle;
		_Rectangle = 0;
	}
	ShutdownBuffers();
}

void Button::ShutdownBuffers()
{
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	if (_vertexTextBuffer)
	{
		_vertexTextBuffer->Release();
		_vertexTextBuffer = 0;
	}

	if (_indexTextBuffer)
	{
		_indexTextBuffer->Release();
		_indexTextBuffer = 0;
	}
}