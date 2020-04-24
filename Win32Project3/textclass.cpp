///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"

Text::Text()
{
	Font_ = nullptr;
	FontShader_ = nullptr;
	sentence_ = nullptr;
	D3dx_ = nullptr;

	scale = 1.0f;
	color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	position = D3DXVECTOR3(0.f, 0.f, 0.f);
}

Text::Text(const Text& other)
{
}

Text::~Text()
{
}

bool Text::Initialize(D3DClass* direct, HWND hwnd)
{
	bool result;

	D3dx_ = direct;

	Font_ = new FontClass;
	if (!Font_)
	{
		return false;
	}

	// Initialize the font object.
	result = Font_->Initialize(direct->GetDevice(), "../Win32Project3/data/fontdata.txt", L"../Win32Project3/data/font.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	FontShader_ = new FontShaderClass;
	if (!FontShader_) return false;
	result = FontShader_->Initialize(direct->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	result = InitializeSentence(_default_length);
	if (!result) return false;

	result = UpdateSentence( " ");
	if (!result) return false;

	return true;
}

bool Text::Initialize(D3DClass* direct, HWND hwnd, string** header, string** data)
{
	bool result;
	int i;
	result = Initialize(direct, hwnd);
	


	//Data
	/*i = 0;
	_Description = new SentenceType*[25];
	for (int j = 0; j<25; j++)
	{
		_Description[j] = 0;
	}

	while (*header && (*header)[i].length()>0 )
	{
		result = InitializeSentence(&_Description[i], 128, direct->GetDevice());
		if (!result)
		{
			return false;
		}

		result = UpdateSentence(_Description[i], (*header)[i] + ": " + (*data)[i], 20, 80 + (20 * i), 1.0f, 1.0f, 0.0f, direct->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		i++;
	}

	result = InitializeSentence(&_gridDensity, 32, direct->GetDevice());
	if (!result)
	{
		return false;
	}

	result = UpdateSentence(_gridDensity, " ", screenWidth - 50, 20, 1.0f, 1.0f, 0.0f, direct->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&_overdriveZ, 32, direct->GetDevice());
	if (!result)
	{
		return false;
	}
	
	result = UpdateSentence(_overdriveZ, " ", screenWidth - 50, 40, 1.0f, 1.0f, 0.0f, direct->GetDeviceContext());
	if (!result)
	{
		return false;
	}*/

	return true;	
}

void Text::Shutdown()
{
	// Release the sentence.
	ReleaseSentence(&sentence_);

	/*if (_Description)
	{
		int i = 0;
		while (i < 25 && _Description[i])
		{
			ReleaseSentence( &_Description[i]);
			i++;
		}
		delete[] _Description;
		_Description = 0;
	}*/

	// Release the font shader object.
	if (FontShader_)
	{
		FontShader_->Shutdown();
		delete FontShader_;
		FontShader_ = 0;
	}

	// Release the font object.
	if (Font_)
	{
		Font_->Shutdown();
		delete Font_;
		Font_ = 0;
	}

	return;
}

bool Text::Render(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;
	result = RenderSentence(worldMatrix, viewMatrix, orthoMatrix);
	return result;
}

bool Text::InitializeSentence(int maxLength)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	sentence_ = new SentenceType;
	if (!sentence_)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	sentence_->vertexBuffer = 0;
	sentence_->indexBuffer = 0;

	// Set the maximum length of the sentence.
	sentence_->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	sentence_->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	sentence_->indexCount = sentence_->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[sentence_->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[sentence_->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence_->vertexCount));

	// Initialize the index array.
	for (i = 0; i< sentence_->indexCount; i++)
	{
		indices[i] = i;
	}

	
	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * sentence_->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = D3dx_->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &sentence_->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * sentence_->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = D3dx_->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &sentence_->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool Text::UpdateSentence(char* text)
{
	int numLetters;
	VertexType* vertices;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	numLetters = (int)strlen(text);

	if (numLetters > sentence_->maxLength) return false;

	vertices = new VertexType[sentence_->vertexCount];
	if (!vertices) return false;

	memset(vertices, 0, (sizeof(VertexType) * sentence_->vertexCount));
	
	Font_->BuildVertexArray((void*)vertices, text);

	result = D3dx_->GetDeviceContext()->Map(sentence_->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence_->vertexCount));

	// Unlock the vertex buffer.
	D3dx_->GetDeviceContext()->Unmap(sentence_->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool Text::UpdateSentence(std::string text)
{
	bool result;
	char *cstr = new char[text.length() + 1];
	strcpy(cstr, text.c_str());

	result = Text::UpdateSentence(cstr);
	
	return result;
}

void Text::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool Text::RenderSentence(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	D3DXMATRIX matrix;
	unsigned int stride, offset;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	D3dx_->GetDeviceContext()->IASetVertexBuffers(0, 1, &sentence_->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	D3dx_->GetDeviceContext()->IASetIndexBuffer(sentence_->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	D3dx_->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMatrixScaling(&matrix, scale, scale, scale);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &matrix);

	/*D3DXMatrixRotationYawPitchRoll(&matrix, rotY, rotX, rotZ);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &matrix);*/

	D3DXMatrixTranslation(&matrix, position.x, position.y, position.z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &matrix);
	result = FontShader_->Render(D3dx_->GetDeviceContext(), sentence_->indexCount, worldMatrix, viewMatrix, orthoMatrix, Font_->GetTexture(),
		color);
	if (!result)
	{
		false;
	}

	return true;
}

bool Text::SetText(std::string text)
{
	return Text::UpdateSentence(text);
}

bool Text::SetText(char* text)
{
	return Text::UpdateSentence(text);
}


