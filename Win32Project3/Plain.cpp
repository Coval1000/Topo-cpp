#include "Plain.h"

Plain::Plain()
{
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
	D3Dx_ = nullptr;
	Vertices_ = nullptr;
	HeightMap_ = nullptr;
	COLOR_MODE_ = ColorMode::GreyScale;
	griddensity_ = 1;
	PLOTTING = DataMode::Extrapolator0;
	_offSetX = 0;
	_offSetY = 0;
	RENDERING = Solid;

}

Plain::Plain(const Plain& other)
{

}

Plain::~Plain()
{
}

D3DXVECTOR4	Plain::HighContrastBuffer(double heigh)
{
	heigh = (heigh - _bottom) / (_top - _bottom);
	double color = (heigh * 7) - (int)(heigh * 7);
	switch ((int)(heigh * 7))
	{
	case 0:
		return D3DXVECTOR4(0.0f, 0.0f, color, 1.0f);
		break;
	case 1:
		return D3DXVECTOR4(0.0f, color, 1.0f, 1.0f);
		break;
	case 2:
		return D3DXVECTOR4(0.0f, 1.0f, 1.0f - color, 1.0f);
		break;
	case 3:
		return D3DXVECTOR4(color, 1.0f, 0.0f, 1.0f);
		break;
	case 4:
		return D3DXVECTOR4(1.0f, 1.0f - color, 0.0f, 1.0f);
		break;
	case 5:
		return D3DXVECTOR4(1.0f, 0.0f, color, 1.0f);
		break;
	case 6:
		return D3DXVECTOR4(1.0f, color, 1.0f, 1.0f);
		break;
	case 7:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	}
}
D3DXVECTOR4	Plain::HybrydBuffer(double heigh)
{
	heigh = (heigh - _bottom) / (_top - _bottom);
	double color = (heigh * 7) - (int)(heigh * 7);
	switch ((int)(heigh * 7))
	{
	case 0:
		return D3DXVECTOR4(0.0f, 0.0f, heigh * color, 1.0f);
		break;
	case 1:
		return D3DXVECTOR4(0.0f, heigh * color, heigh, 1.0f);
		break;
	case 2:
		return D3DXVECTOR4(0.0f, heigh, heigh * (1.0f - color), 1.0f);
		break;
	case 3:
		return D3DXVECTOR4(heigh * color, heigh, 0.0f, 1.0f);
		break;
	case 4:
		return D3DXVECTOR4(heigh, heigh * (1.0f - color), 0.0f, 1.0f);
		break;
	case 5:
		return D3DXVECTOR4(heigh, 0.0f, heigh * color, 1.0f);
		break;
	case 6:
		return D3DXVECTOR4(heigh, heigh * color, heigh, 1.0f);
		break;
	case 7:
		return D3DXVECTOR4(heigh, heigh, heigh, 1.0f);
		break;
	default:
		return D3DXVECTOR4(heigh, heigh, heigh, 0.0f);
		break;
	}
}
D3DXVECTOR4	Plain::GreyScaleBuffer(double heigh)
{
	heigh = (heigh - _bottom) / (_top - _bottom);

	return D3DXVECTOR4(heigh, heigh, heigh, 1.0f);

}
D3DXVECTOR4	Plain::SolidBuffer(double heigh)
{
	return D3DXVECTOR4(0.0f, 0.75f, 0.75f, 1.0f);
}
D3DXVECTOR4	Plain::FullBitBuffer(double heigh)
{
	heigh = (heigh - _bottom) / (_top - _bottom);
	double color = (heigh * 7) - (int)(heigh * 7);
	switch ((int)(heigh * 7))
	{
	case 0:
		return D3DXVECTOR4(0.0f, 0.0f, color, 1.0f);
		break;
	case 1:
		return D3DXVECTOR4(0.0f, color, 1.0f - color, 1.0f);
		break;
	case 2:
		return D3DXVECTOR4(0.0f, 1.0f, color, 1.0f);
		break;
	case 3:
		return D3DXVECTOR4(color, 1.0f - color, 1.0f - color, 1.0f);
		break;
	case 4:
		return D3DXVECTOR4(1.0f, 0.0f, color, 1.0f);
		break;
	case 5:
		return D3DXVECTOR4(1.0f, color, 1.0f - color, 1.0f);
		break;
	case 6:
		return D3DXVECTOR4(1.0f, 1.0f, color, 1.0f);
		break;
	case 7:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	}
}
D3DXVECTOR4	Plain::FullBitEyeCorrectBuffer(double heigh)
{
	heigh = (heigh - _bottom) / (_top - _bottom);
	double color = (heigh * 7) - (int)(heigh * 7);
	switch ((int)(heigh * 7))
	{
	case 0:
		return D3DXVECTOR4(0.0f, 0.0f, color, 1.0f);
		break;
	case 1:
		return D3DXVECTOR4( color, 0.0f, 1.0f - color, 1.0f);
		break;
	case 2:
		return D3DXVECTOR4( 1.0f, 0.0f,color, 1.0f);
		break;
	case 3:
		return D3DXVECTOR4( 1.0f - color, color, 1.0f - color, 1.0f);
		break;
	case 4:
		return D3DXVECTOR4(0.0f, 1.0f, color, 1.0f);
		break;
	case 5:
		return D3DXVECTOR4(color, 1.0f, 1.0f - color, 1.0f);
		break;
	case 6:
		return D3DXVECTOR4(1.0f, 1.0f, color, 1.0f);
		break;
	case 7:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	}
}

int Plain::GetTerrainWidth()
{
	return _terrainWidth;
}
int Plain::GetTerrainHeight()
{
	return _terrainHeight;
}
int Plain::GetGridDensity()
{
	return griddensity_;
}
int Plain::GetIndexCount()
{
	return _indexCount;
}
int Plain::GetOffSetX()
{
	return _offSetX;
}
int Plain::GetOffSetY()
{
	return _offSetY;
}
float Plain::GetTerrainValue(int x, int y)
{
	if (!Vertices_)
	{
		return 0;
	}
	return Vertices_[(_terrainHeight * x) + y].position.y;
}

bool Plain::Initialize(D3DClass* d3dx, D3DXVECTOR3* heightMap, int width, int height, float top, float bottom, RenderStyle REN, ColorMode colorMode)
{
	bool result;
	D3Dx_ = d3dx;
	_terrainWidth = width;
	_terrainHeight = height;
	_top = top;
	_bottom = bottom;
	HeightMap_ = new D3DXVECTOR3[_terrainWidth * _terrainHeight];
	if (!HeightMap_) return false;
	memcpy(HeightMap_, heightMap, sizeof(D3DXVECTOR3) * _terrainWidth * _terrainHeight);
	RENDERING = REN;
	if (colorMode != Undefined) COLOR_MODE_ = colorMode;
	result = InitializeBuffers();
	if (!result) return false;
	return true;
}
bool Plain::InitializeBuffers()
{
	unsigned long** indices;
	D3D11_BUFFER_DESC indexBufferGridDesc;
	D3D11_SUBRESOURCE_DATA indexGridData;
	HRESULT result;

	indices = new unsigned long*;

	if (!InitializePlot(indices)) return false;
	UpdateColor(Vertices_);
	//UpdateGridIndices(*indicesGrid);

	result = FillVertex();
	if (FAILED(result)) return false;
	if (RENDERING == Solid)
	{
		result = FillIndex(*indices);
		if (FAILED(result)) return false;
	}
	else
	{
		result = FillIndexGrid(*indices);
		if (FAILED(result)) return false;
	}

	delete[] * indices;
	delete indices;
	indices = nullptr;
	return true;
}
bool Plain::InitializePlot(unsigned long** indices)
{
	int k = 0;
	switch (PLOTTING)
	{
	case Extrapolator0:
		k = 4;
		break;
	case Extrapolator1:
		k = 1;
		break;
	case Extrapolator1Interpolated:
		k = 4;
		break;
	}
	_vertexCount = (_terrainWidth) * (_terrainHeight)* k;

	if(RENDERING == Solid) _indexCount = _vertexCount * 6;
	else _indexCount = _vertexCount * 6;

	if (Vertices_)
	{
		delete[] Vertices_;
		Vertices_ = nullptr;
	}
	Vertices_ = new VertexType[_vertexCount];
	if (!Vertices_) return false;

	std::memset(Vertices_, 0, (sizeof(VertexType) * _vertexCount));

	*indices = new unsigned long[_indexCount];
	if (!*indices) return false;
	std::memset(*indices, 0, _indexCount);

	InitializeVertices();
	InitializeIndices(*indices);
	return true;
}
void Plain::InitializeVertices()
{
	unsigned long index = 0;
	switch (PLOTTING)
	{
	case Extrapolator0:
		for (int j = 0; j < (_terrainHeight); j++)
		{
			for (int i = 0; i < (_terrainWidth); i++)
			{
				index = ((_terrainHeight * j) + i);
				Vertices_[index * 4].position = D3DXVECTOR3(HeightMap_[index].x - 0.5, HeightMap_[index].y, HeightMap_[index].z - 0.5);//UL
				Vertices_[index * 4 + 1].position = D3DXVECTOR3(HeightMap_[index].x + 0.5, HeightMap_[index].y, HeightMap_[index].z - 0.5);//UR
				Vertices_[index * 4 + 2].position = D3DXVECTOR3(HeightMap_[index].x - 0.5, HeightMap_[index].y, HeightMap_[index].z + 0.5);//BL
				Vertices_[index * 4 + 3].position = D3DXVECTOR3(HeightMap_[index].x + 0.5, HeightMap_[index].y, HeightMap_[index].z + 0.5);//BR
			}
		}
		break;
	case Extrapolator1:
		for (int j = 0; j < (_terrainHeight); j++)
		{
			for (int i = 0; i < (_terrainWidth); i++)
			{
				index = (_terrainHeight * j) + i;
				Vertices_[index].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);
			}
		}
		break;
	case Extrapolator1Interpolated:
		for (int j = 0; j < (_terrainHeight - 1); j++)
		{
			for (int i = 0; i < (_terrainWidth - 1); i++)
			{
				index = ((_terrainHeight * j) + i);
				Vertices_[index * 4].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
				Vertices_[index * 4 + 1].position = D3DXVECTOR3(HeightMap_[index].x + 0.5f, (HeightMap_[index].y + HeightMap_[index + 1].y) / 2.f, HeightMap_[index].z);//UR
				Vertices_[index * 4 + 2].position = D3DXVECTOR3(HeightMap_[index].x, (HeightMap_[index].y + HeightMap_[index + _terrainHeight].y) / 2.f, HeightMap_[index].z + 0.5f);//BL
				Vertices_[index * 4 + 3].position = D3DXVECTOR3(HeightMap_[index].x + 0.5f, (HeightMap_[index].y + HeightMap_[index + _terrainHeight].y + HeightMap_[index + 1].y + HeightMap_[index + _terrainHeight + 1].y) / 4.f, HeightMap_[index].z + 0.5f);//BR
			}
			index = ((_terrainHeight * j) + _terrainWidth - 1);
			Vertices_[index * 4].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
			Vertices_[index * 4 + 1].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
			Vertices_[index * 4 + 2].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
			Vertices_[index * 4 + 3].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
		}
		for (int i = 0; i < (_terrainWidth - 1); i++)
		{
			index = ((_terrainHeight * (_terrainWidth - 1)) + i);
			Vertices_[index * 4].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
			Vertices_[index * 4 + 1].position = D3DXVECTOR3(HeightMap_[index].x + 0.5f, (HeightMap_[index].y + HeightMap_[index + 1].y) / 2.f, HeightMap_[index].z);//UR
			Vertices_[index * 4 + 2].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//BL
			Vertices_[index * 4 + 3].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//BR
		}
		++index;
		Vertices_[index * 4].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
		Vertices_[index * 4 + 1].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
		Vertices_[index * 4 + 2].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
		Vertices_[index * 4 + 3].position = D3DXVECTOR3(HeightMap_[index].x, HeightMap_[index].y, HeightMap_[index].z);//UL
		break;
	case Raw:
		break;
	}
}
void Plain::InitializeIndices(unsigned long* indices)
{
	switch (RENDERING)
	{
	case Solid:

		switch (PLOTTING)
		{
		case Extrapolator0:
			IndexRaw(indices);
			break;
		case Extrapolator1:
			IndexInterpolated0(indices);
			break;
		case Extrapolator1Interpolated:
			IndexInterpolated1(indices);
			break;
		case Raw:
			break;
		}
		break;

	case Grid:

		switch (PLOTTING)
		{
		case Extrapolator0:
			IndexGridRaw(indices);
			break;
		case Extrapolator1:
			IndexGridInterpolated0(indices);
			break;
		case Extrapolator1Interpolated:
			IndexGridInterpolated1(indices);
			break;
		}
		break;
	}
}

void Plain::IndexRaw(unsigned long* indices)
{
	unsigned long index = -1, node[4];
	for (int y = 0; y < (_terrainHeight - 1); ++y)
	{
		for (int x = 0; x < (_terrainWidth - 1); ++x)
		{
			node[0] = ((_terrainHeight * y) + x) * 4;          // Upper left.
			node[1] = ((_terrainHeight * y) + (x + 1)) * 4;      // Upper right.
			node[2] = ((_terrainHeight * (y + 1)) + x) * 4;      // Bottom left.

			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 0;
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 3;
			indices[++index] = node[0] + 1;

			indices[++index] = node[1] + 2;
			indices[++index] = node[1] + 0;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[1] + 2;

			indices[++index] = node[0] + 2;
			indices[++index] = node[2] + 0;
			indices[++index] = node[2] + 1;
			indices[++index] = node[2] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[0] + 2;

		}
		node[0] = ((_terrainHeight * y) + _terrainWidth - 1) * 4;          // Upper left.
		node[2] = ((_terrainHeight * (y + 1)) + _terrainWidth - 1) * 4;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 0;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 1;

		indices[++index] = node[0] + 2;
		indices[++index] = node[2] + 0;
		indices[++index] = node[2] + 1;
		indices[++index] = node[2] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 2;

	}

	for (int x = 0; x < (_terrainWidth - 1); ++x)
	{
		node[0] = (_terrainHeight * (_terrainWidth - 1) + x) * 4;          // Upper left. x,y
		node[1] = ((_terrainHeight * (_terrainWidth - 1)) + (x + 1)) * 4;      // Upper right. x+1,y

		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 0;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 1;

		indices[++index] = node[1] + 2;
		indices[++index] = node[1] + 0;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[1] + 2;

	}
	node[0] = (_terrainHeight * _terrainWidth) * 4;          // Upper left. x,y

	indices[++index] = node[0] + 1;
	indices[++index] = node[0] + 0;
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 3;
	indices[++index] = node[0] + 1;
	return;
}
void Plain::IndexGridRaw(unsigned long* indices)
{
	unsigned long index = -1, node[4];
	for (int y = 0; y < (_terrainHeight - 1); ++y)
	{
		for (int x = 0; x < (_terrainWidth - 1); ++x)
		{
			node[0] = ((_terrainHeight * y) + x) * 4;          // Upper left.
			node[1] = ((_terrainHeight * y) + (x + 1)) * 4;      // Upper right.
			node[2] = ((_terrainHeight * (y + 1)) + x) * 4;      // Bottom left.

			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 0;
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 3;
			indices[++index] = node[0] + 1;

			indices[++index] = node[1] + 2;
			indices[++index] = node[1] + 0;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[1] + 2;

			indices[++index] = node[0] + 2;
			indices[++index] = node[2] + 0;
			indices[++index] = node[2] + 1;
			indices[++index] = node[2] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[0] + 2;

		}
		node[0] = ((_terrainHeight * y) + _terrainWidth - 1) * 4;          // Upper left.
		node[2] = ((_terrainHeight * (y + 1)) + _terrainWidth - 1) * 4;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 0;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 1;

		indices[++index] = node[0] + 2;
		indices[++index] = node[2] + 0;
		indices[++index] = node[2] + 1;
		indices[++index] = node[2] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 2;

	}

	for (int x = 0; x < (_terrainWidth - 1); ++x)
	{
		node[0] = (_terrainHeight * (_terrainWidth - 1) + x) * 4;          // Upper left. x,y
		node[1] = ((_terrainHeight * (_terrainWidth - 1)) + (x + 1)) * 4;      // Upper right. x+1,y

		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 0;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 1;

		indices[++index] = node[1] + 2;
		indices[++index] = node[1] + 0;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[1] + 2;

	}
	node[0] = (_terrainHeight * _terrainWidth) * 4;          // Upper left. x,y

	indices[++index] = node[0] + 1;
	indices[++index] = node[0] + 0;
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 3;
	indices[++index] = node[0] + 1;
	return;
}
void Plain::IndexInterpolated0(unsigned long* indices)
{
	unsigned long index = -1, node[4];
	for (int j = 0; j < (_terrainHeight - 1); j++)
	{
		for (int i = 0; i < (_terrainWidth - 1); i++)
		{
			node[0] = (_terrainHeight * j) + i;          // Upper left. a
			node[1] = (_terrainHeight * j) + (i + 1);      // Upper right. a+1
			node[2] = (_terrainHeight * (j + 1)) + i;      // Bottom left. x+a
			node[3] = (_terrainHeight * (j + 1)) + (i + 1);  // Bottom right.  x+a+1

			indices[++index] = node[2];
			// Upper right. x+a+1
			indices[++index] = node[3];
			// Bottom left. a
			indices[++index] = node[0];
			// Bottom left. a
			indices[++index] = node[0];
			// Upper right. x+a+1
			indices[++index] = node[3];
			// Bottom right. a+1
			indices[++index] = node[1];

		}
	}
	return;
}
void Plain::IndexGridInterpolated0(unsigned long* indices)
{
	unsigned long index = -1, node[4];
	for (int j = 0; j < (_terrainHeight); j = j + griddensity_)
	{
		for (int i = 0; i < (_terrainWidth - 1); ++i)
		{
			node[0] = (_terrainHeight * j) + i;
			node[1] = (_terrainHeight * j) + i + 1;

			indices[++index] = node[0];
			indices[++index] = node[1];
		}
	}

	for (int j = 0; j < (_terrainWidth); j = j + griddensity_)
	{
		for (int i = 0; i < (_terrainHeight - 1); ++i)
		{
			node[0] = (_terrainHeight * i) + j;
			node[1] = (_terrainHeight * (i + 1)) + j;

			indices[++index] = node[0];
			indices[++index] = node[1];
		}
	}
}
void Plain::IndexInterpolated1(unsigned long* indices)
{
	unsigned long index = -1, node[4];
	for (int y = 0; y < (_terrainHeight - 1); ++y)
	{
		for (int x = 0; x < (_terrainWidth - 1); ++x)
		{
			node[0] = ((_terrainHeight * y) + x) * 4;          // Upper left.
			node[1] = ((_terrainHeight * y) + (x + 1)) * 4;      // Upper right.
			node[2] = ((_terrainHeight * (y + 1)) + x) * 4;      // Bottom left.
			node[3] = ((_terrainHeight * (y + 1)) + x + 1) * 4;      // Bottom left.

			indices[++index] = node[0];
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 3;

			indices[++index] = node[1] + 2;
			indices[++index] = node[1];
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[1] + 2;

			indices[++index] = node[0] + 2;
			indices[++index] = node[2];
			indices[++index] = node[2] + 1;
			indices[++index] = node[2] + 1;
			indices[++index] = node[0] + 3;
			indices[++index] = node[0] + 2;

			indices[++index] = node[0] + 3;
			indices[++index] = node[2] + 1;
			indices[++index] = node[3];
			indices[++index] = node[3];
			indices[++index] = node[1] + 2;
			indices[++index] = node[0] + 3;

		}
		node[0] = ((_terrainHeight * y) + _terrainWidth - 1) * 4;          // Upper left.
		node[2] = ((_terrainHeight * (y + 1)) + _terrainWidth - 1) * 4;
		indices[++index] = node[0];
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;

		indices[++index] = node[0] + 2;
		indices[++index] = node[2];
		indices[++index] = node[2] + 1;
		indices[++index] = node[2] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[0] + 2;

	}

	for (int x = 0; x < (_terrainWidth - 1); ++x)
	{
		node[0] = (_terrainHeight * (_terrainWidth - 1) + x) * 4;          // Upper left. x,y
		node[1] = ((_terrainHeight * (_terrainWidth - 1)) + (x + 1)) * 4;      // Upper right. x+1,y

		indices[++index] = node[0];
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 3;

		indices[++index] = node[1] + 2;
		indices[++index] = node[1];
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 3;
		indices[++index] = node[1] + 2;
		indices[++index] = node[0] + 1;

	}
	node[0] = (_terrainHeight * _terrainWidth) * 4;          // Upper left. x,y

	indices[++index] = node[0];
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 1;
	indices[++index] = node[0] + 1;
	indices[++index] = node[0] + 2;
	indices[++index] = node[0] + 3;
	return;
}
void Plain::IndexGridInterpolated1(unsigned long* indices)
{
	unsigned long index = -1;
	unsigned long node[4];
	for (int y = 0; y < (_terrainHeight - 1); ++y)
	{
		for (int x = 0; x < (_terrainWidth - 1); ++x)
		{
			node[0] = (unsigned long)(((_terrainHeight * y) + x) * 4);          // Upper left.
			node[1] = (unsigned long)(((_terrainHeight * y) + (x + 1)) * 4);      // Upper right.
			node[2] = (unsigned long)(((_terrainHeight * (y + 1)) + x) * 4);      // Bottom left.
			node[3] = (unsigned long)(((_terrainHeight * (y + 1)) + x + 1) * 4);      // Bottom left.

			indices[++index] = node[0];
			indices[++index] = node[0] + 2;
			indices[++index] = node[0] + 2;
			indices[++index] = node[2];

			indices[++index] = node[0];
			indices[++index] = node[0] + 1;
			indices[++index] = node[0] + 1;
			indices[++index] = node[1];

		}
		node[0] = (unsigned long)((_terrainHeight * y) + _terrainWidth - 1) * 4;          // Upper left.
		node[2] = (unsigned long)((_terrainHeight * (y + 1)) + _terrainWidth - 1) * 4;
		indices[++index] = node[0];
		indices[++index] = node[0] + 2;
		indices[++index] = node[0] + 2;
		indices[++index] = node[2];

		indices[++index] = node[0];
		indices[++index] = node[0] + 1;
	}

	for (int x = 0; x < (_terrainWidth - 1); ++x)
	{
		node[0] = (unsigned long)(_terrainHeight * (_terrainWidth - 1) + x) * 4;          // Upper left. x,y
		node[1] = (unsigned long)((_terrainHeight * (_terrainWidth - 1)) + (x + 1)) * 4;      // Upper right. x+1,y

		indices[++index] = node[0];
		indices[++index] = node[0] + 2;

		indices[++index] = node[0];
		indices[++index] = node[0] + 1;
		indices[++index] = node[0] + 1;
		indices[++index] = node[1];

	}
	node[0] = (unsigned long)(_terrainHeight * _terrainWidth) * 4;          // Upper left. x,y

	indices[++index] = node[0];
	indices[++index] = node[0] + 2;

	indices[++index] = node[0];
	indices[++index] = node[0] + 1;
	return;
}

HRESULT Plain::FillVertex()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	std::ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	std::ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = Vertices_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	return D3Dx_->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
}
HRESULT Plain::FillIndex(unsigned long* indices)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	return D3Dx_->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
}
HRESULT Plain::FillIndexGrid(unsigned long* indicesGrid)
{
	D3D11_BUFFER_DESC indexBufferGridDesc;
	D3D11_SUBRESOURCE_DATA indexGridData;

	indexBufferGridDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferGridDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferGridDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferGridDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferGridDesc.MiscFlags = 0;
	indexBufferGridDesc.StructureByteStride = 0;

	indexGridData.pSysMem = indicesGrid;
	indexGridData.SysMemPitch = 0;
	indexGridData.SysMemSlicePitch = 0;

	return D3Dx_->GetDevice()->CreateBuffer(&indexBufferGridDesc, &indexGridData, &_indexBuffer);
}

bool Plain::SetMode(Plain::DataMode mode)
{
	PLOTTING = mode;
	ShutdownBuffers();
	return InitializeBuffers();
}

void Plain::ShutdownBuffers()
{
	// Release the index buffer.
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

void Plain::Render()
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	D3Dx_->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	D3Dx_->GetDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3Dx_->GetDeviceContext()->IASetPrimitiveTopology(RENDERING == Solid ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	return;
}

bool Plain::Update(bool updateColor, bool updateVertex, bool updateIndices)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (updateColor || updateVertex)
	{

		if (!Vertices_)
		{
			return false;
		}

		if (updateVertex)UpdateTerrain(Vertices_);

		if (updateColor)UpdateColor(Vertices_);

		result = D3Dx_->GetDeviceContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) return false;
		memcpy(mappedResource.pData, (void*)Vertices_, (sizeof(VertexType) * _vertexCount));
		D3Dx_->GetDeviceContext()->Unmap(_vertexBuffer, 0);
	}

	if (updateIndices)
	{
		//unsigned long* indices;
		//indices = new unsigned long[_indexGridCount];
		//if (!indices)
		//{
		//	return false;
		//}

		////UpdateGridIndices(indices);

		//result = D3Dx_->GetDeviceContext()->Map(_indexGridBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//if (FAILED(result))
		//{
		//	delete[] indices;
		//	indices = 0;
		//	return false;
		//}

		//// Copy the data into the vertex buffer.
		//memcpy(mappedResource.pData, (void*)indices, (sizeof(unsigned long) * _indexGridCount));

		//// Unlock the vertex buffer.
		//D3Dx_->GetDeviceContext()->Unmap(_indexGridBuffer, 0);
		//delete[] indices;
		//indices = 0;
	}


	return true;
}
bool Plain::UpdateTerrain(VertexType* vertices)
{
	int  index, i, j;

	for (j = 0; j < (_terrainHeight); j++)
	{
		for (i = 0; i < (_terrainWidth); i++)
		{
			index = (_terrainHeight * j) + i;

			vertices[index].position.y = HeightMap_[index].y;

		}
	}

	////
	return true;
}
bool Plain::UpdateColor(VertexType* vertices)
{
	int  index, i, j;
	int k;
	switch (PLOTTING)
	{
	case Extrapolator0:
		k = 2;
		break;
	case Extrapolator1:
		k = 1;
		break;
	case Extrapolator1Interpolated:
		k = 2;
		break;
	}
	for (j = 0; j < (_terrainHeight * k); j++)
	{
		for (i = 0; i < (_terrainWidth * k); i++)
		{
			index = (_terrainHeight * j * k) + i;

			switch (COLOR_MODE_)
			{
			case ColorMode::HighContrast:
				vertices[index].color = HighContrastBuffer(Vertices_[index].position.y);
				break;

			case HCGSHybrid:
				vertices[index].color = HybrydBuffer(Vertices_[index].position.y);
				break;
			case SolidColor:
				vertices[index].color = SolidBuffer(Vertices_[index].position.y);
				break;

			case FullBit:
				vertices[index].color = FullBitBuffer(Vertices_[index].position.y);
				break;
			case FullBitEyeCorrect:
				vertices[index].color = FullBitEyeCorrectBuffer(Vertices_[index].position.y);
				break;
			default:
				vertices[index].color = GreyScaleBuffer(Vertices_[index].position.y);
				break;
			}

		}
	}
	/*if (ClickPoint_.x >= 0)
	{
		vertices[(ClickPoint_.x * terrainHeight_) + ClickPoint_.y].color = D3DXVECTOR4(0.9f, 0.0f, 0.1f, 1.0f);
	}*/
	return true;
}

bool Plain::ChangeColorMode(ColorMode colormode)
{
	bool result;
	if (colormode != Undefined) COLOR_MODE_ = colormode;
	result = Update(true, false, false);
	return result;
}

bool Plain::ChangeGridSize(int gridSize)
{
	bool result;
	if (griddensity_ + gridSize > 0)
		griddensity_ += gridSize;
	result = Update(false, false, true);
	return result;

}

void Plain::Shutdown()
{
	ShutdownBuffers();
	delete[] Vertices_;
	Vertices_ = 0;

	return;
}