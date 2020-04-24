////////////////////////////////////////////////////////////////////////////////
// Filename: VectorLineClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "VectorLineClass.h"

//##########
//Initialize
//##########
VectorLineClass::VectorLineClass()
{
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
	_Direct3D = nullptr;
	draw = true;
	Point_ = nullptr;
	Shader_ = nullptr;
	drawtype = DRAW_TYPE::List;
	vertices_count_ = 2;
	indices_ = nullptr;
}

VectorLineClass::VectorLineClass(const VectorLineClass& other)
{
}

VectorLineClass::~VectorLineClass()
{
	//Shutdown();
	Shader_ = 0;
	_Direct3D = 0;
}

bool VectorLineClass::Initialize(D3DClass *Direct, HWND hwnd)
{
	_Direct3D = Direct;
	Shader_ = new MonocolorShaderClass;
	if (!_Direct3D && !Shader_) return false;
	if (Shader_->Initialize(_Direct3D->GetDevice(), hwnd));
	{
		Point_ = new VertexType[vertices_count_];
		if (!Point_) return false;
		indices_ = new int[vertices_count_];
		if (!indices_) return false;

		Point_[0].position = D3DXVECTOR3(0.f, 0.f, 0.f);
		Point_[1].position = D3DXVECTOR3(0.f, 0.f, 0.f);
		indices_[0] = 0;
		indices_[1] = 1;
		return InitializeBuffers();
	}
	return false;
}

bool VectorLineClass::InitializeBuffers()
{
	ID3D11Device* device;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ID3D11Buffer *vertexB;
	HRESULT result;
	device = _Direct3D->GetDevice();

	

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertices_count_ * 2;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = Point_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexB);
	if (FAILED(result))	return false;
	_vertexBuffer = vertexB;

	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(int) * vertices_count_ * 2;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices_;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result)) return false;
	isBufferBigEnough = true;
	return true;
}
//######
//Render
//######
bool VectorLineClass::Render(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (draw)
	{
		RenderBuffers();
		return Shader_->Render(_Direct3D->GetDeviceContext(), vertices_count_, worldMatrix, viewMatrix, projectionMatrix, Color);
	}
	return true;
}

bool VectorLineClass::SetPoint(int index, D3DXVECTOR3 pos)
{
	if (index >= vertices_count_) return false;
	Point_[index].position = pos;
	return UpdateBuffer();
}
bool VectorLineClass::SetPoint(int index, float x, float y, float z)
{
	return SetPoint(index, D3DXVECTOR3(x,y,z));
}

bool VectorLineClass::SetPoints(int count, D3DXVECTOR3* points)
{
	if (!Point_) return false;
	vertices_count_ = count;
	delete[] indices_;
	delete[] Point_;
	if (vertices_count_ != count)
	{
		isBufferBigEnough = false;
		vertices_count_ = count;
	}
	Point_ = new VertexType[vertices_count_];
	indices_ = new int[vertices_count_];
	for ( int i = 0; i < vertices_count_; ++i)
	{
		Point_[i].position = points[i];
		indices_[i] = i;
	}
	return UpdateBuffer();
}

bool VectorLineClass::SetPoints(int count, D3DXVECTOR3 point... )
{
	va_list args;
	va_start(args, count);

	if (!Point_) { va_end(args); return false; }
	vertices_count_ = count;
	delete[] indices_;
	delete[] Point_;
	if (vertices_count_ != count)
	{
		isBufferBigEnough = false;
		vertices_count_ = count;
	}
	Point_ = new VertexType[vertices_count_];
	indices_ = new int[vertices_count_];
	for (int i = 0; i < vertices_count_; ++i)
	{
		Point_[i].position = va_arg(args, D3DXVECTOR3);
		indices_[i] = i;
	}
	va_end(args);
	return UpdateBuffer();
}

void VectorLineClass::RenderBuffers()
{
	ID3D11DeviceContext* deviceContext;
	unsigned int stride;
	unsigned int offset;

	deviceContext = _Direct3D->GetDeviceContext();
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)drawtype);
	return;
}

//###
//Get
//###
int VectorLineClass::GetIndexCount()
{
	return vertices_count_;
}

//########
//Shutdown
//########
void VectorLineClass::Shutdown()
{
	ShutdownBuffers();
	return;
}

void VectorLineClass::ShutdownBuffers()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}

	return;
}

//######
//Update
//######

bool VectorLineClass::UpdateBuffer()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (isBufferBigEnough)
	{
		result = _Direct3D->GetDeviceContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) return false;
		memcpy(mappedResource.pData, (void*)Point_, (sizeof(VertexType) * vertices_count_));
		_Direct3D->GetDeviceContext()->Unmap(_vertexBuffer, 0);

		result = _Direct3D->GetDeviceContext()->Map(_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) return false;
		memcpy(mappedResource.pData, (void*)indices_, (sizeof(int) * vertices_count_));
		_Direct3D->GetDeviceContext()->Unmap(_indexBuffer, 0);
	}
	else
	{
		ShutdownBuffers();
		InitializeBuffers();//todo: add error handling
	}
	return true;
}

