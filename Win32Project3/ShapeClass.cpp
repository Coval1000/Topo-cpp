///////////////////////////////////////////////////////////////////////////////
// Filename: ShapeleClass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "ShapeClass.h"

ShapeClass::ShapeClass(int indices,int vertices) : indicesCount(indices), verticesCount(vertices)
{	
	vertexBuffer_ = 0;
	indexBuffer_ = 0;
	Shader_ = 0;
	_Direct3D = 0;
	draw = true;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	width = 1;
	height = 1;
	x = 0;
	y = 0;
	z = 0;
	twoWay = false;
}

ShapeClass::~ShapeClass()
{
	Shutdown();
}

void ShapeClass::Shutdown()
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = 0;
	}

	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = 0;
	}
}

bool ShapeClass::Initialize(D3DClass *direct3D, HWND hwnd)
{
	_Direct3D = direct3D;

	Shader_ = new MonocolorShaderClass;
	if (!Shader_) return false;
	if(Shader_->Initialize(_Direct3D->GetDevice(), hwnd));
	{
		return InitializeBuffor();	
	}

	MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	return false;
}

bool ShapeClass::InitializeBuffor()
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	ID3D11Device* device = _Direct3D->GetDevice();
	VertexType *vertices;

	vertices = new VertexType[verticesCount];
	if (!vertices)
	{
		return false;
	}

	std::memset(vertices, 0, (sizeof(VertexType) * verticesCount));

	indices = new unsigned long[indicesCount];
	if (!indices)
	{
		return false;
	}

	std::memset(indices, 0, (sizeof(unsigned long) * indicesCount));

	CreateVertices(vertices);
	CreateIndices(indices);
	std::ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	std::ZeroMemory(&vertexData, sizeof(vertexData));

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_);
	if (FAILED(result))
	{
		return false;
	}

	delete[] indices;
	indices = 0;

	delete[] vertices;
	vertices = 0;
	return true;
}

//######
//Render
//######
bool ShapeClass::Render(D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX orthoMatrix)
{
	if (!draw) return true;
	bool result;
	D3DXMATRIX translateMatrix;
	//bufor
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	ID3D11DeviceContext* deviceContext = _Direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//render

	D3DXMatrixScaling(&translateMatrix, width, height, 1);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	D3DXMatrixRotationYawPitchRoll(&translateMatrix, rotY, rotX, rotZ);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	D3DXMatrixTranslation(&translateMatrix, x, y, z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	result = Shader_->Render(_Direct3D->GetDeviceContext(), twoWay ? indicesCount : indicesCount / 2, worldMatrix, baseViewMatrix, orthoMatrix, color_);

	return result;
}

//########
//SetColor
//########
bool ShapeClass::UpdateBuffor()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	VertexType *vertices;
	vertices = new VertexType[verticesCount];
	CreateVertices(vertices);

	result = _Direct3D->GetDeviceContext()->Map(vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the data into the vertex buffer.
	memcpy(mappedResource.pData, (void*)vertices, (sizeof(VertexType) * verticesCount));

	// Unlock the vertex buffer.
	_Direct3D->GetDeviceContext()->Unmap(vertexBuffer_, 0);

	delete[] vertices;
	vertices = 0;
	return true;
}

bool ShapeClass::SetColor(float R, float G, float B, float A)
{
	color_ = D3DXVECTOR4(R, G, B, A);
	return UpdateBuffor();
}
bool ShapeClass::SetColor(D3DXVECTOR4 color)
{
	color_ = color;
	return UpdateBuffor();
}
bool ShapeClass::SetColor(D3DXVECTOR3 color)
{
	color_ = D3DXVECTOR4(color.x, color.y, color.z, 1.0f);
	return UpdateBuffor();
}
bool ShapeClass::SetColor(D3DXVECTOR3 color, float alpha)
{
	color_ = D3DXVECTOR4(color.x, color.y, color.z, alpha);
	return UpdateBuffor();
}
bool ShapeClass::SetColor(int R, int G, int B, int A)
{
	color_ = D3DXVECTOR4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
	return UpdateBuffor();
}
bool ShapeClass::SetColor(int R, int G, int B)
{
	color_ = D3DXVECTOR4(R / 255.0f, G / 255.0f, B / 255.0f, 1.0f);
	return UpdateBuffor();
}
bool ShapeClass::SetColor(D3DXVECTOR3 color, int alpha)
{
	color_ = D3DXVECTOR4(color.x, color.y, color.z, alpha / 255.0f);
	return UpdateBuffor();
}
