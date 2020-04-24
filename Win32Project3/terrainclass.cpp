////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"


TerrainClass::TerrainClass()
{
	ClickPoint_ = { -1,-1 };
}

TerrainClass::TerrainClass(const TerrainClass& other)
{
}

TerrainClass::~TerrainClass()
{
}

float TerrainClass::GetTop()
{
	return _top;
}

float TerrainClass::GetBottom()
{
	return _bottom;
}

bool TerrainClass::LoadComMap()
{
	HRESULT hr;
	IClassFactory* pICF = nullptr;
	IMesh* pIMesh = nullptr;
	Data* meshData = nullptr;

	hr = CoGetClassObject(CLSID_Agent, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&pICF);
	if (FAILED(hr)) return false;

	hr = pICF->CreateInstance(NULL, IID_IMesh, (void**)&pIMesh);
	if (FAILED(hr))	return false;

	meshData = new Data();
	if (meshData == nullptr) 
	{ 
		pICF->Release();
		return false; 
	}
	pIMesh->GetData(meshData);
	if (meshData->points == nullptr)
	{
		delete meshData;
		return false;
	}

	terrainWidth_ = meshData->sizeX;
	terrainHeight_ = meshData->sizeY;
	if (HeightMap_)
	{
		delete[] HeightMap_;
		HeightMap_ = nullptr;
	}
	HeightMap_ = new D3DXVECTOR3[terrainWidth_ * terrainHeight_];
	if (!HeightMap_)
	{
		return false;
	}


	_top = -1000;
	_bottom = 1000;
	for (int j = 0, index = 0; j<terrainHeight_; ++j)
	{
		for (int i = 0; i<terrainWidth_; ++i, ++index)
		{

			HeightMap_[index].x = (float)i;
			pIMesh->GetCell(&index, &(HeightMap_[index].y));
			HeightMap_[index].z = (float)j;
			if (HeightMap_[index].y > _top) 
			{
				 if (isinf(HeightMap_[index].y))
				{
					_top = FLT_MAX;
				}
				else _top = HeightMap_[index].y;
			}
			else if (HeightMap_[index].y < _bottom)
			{ 
				if (isinf(HeightMap_[index].y))
				{
					_bottom = FLT_MIN;
				}
				else _bottom = HeightMap_[index].y;
			}
			
		}
	}



	//Update(true,true,false);

	delete meshData;
	pICF->Release();
	pIMesh->Release();
}

bool TerrainClass::Initialize(D3DClass* d3dx, string** header, string** data, LPWSTR heightMapFilename, Plain::ColorMode colorMode)
{
	bool result;
	D3Dx_ = d3dx;

	result = LoadHeightMap(header, data, heightMapFilename);
	if (!result) return false;
	result = Grid.Initialize(d3dx, HeightMap_, terrainWidth_, terrainHeight_, _top, _bottom, Plain::Grid, Plain::SolidColor);
	result = Plot.Initialize(d3dx, HeightMap_, terrainWidth_, terrainHeight_, _top, _bottom, Plain::Solid, colorMode);

	
	return true;
}

bool TerrainClass::Initialize(D3DClass* d3dx, int width, int height, D3DXVECTOR3* heightMap, Plain::ColorMode colorMode)
{
	bool result;
	D3Dx_ = d3dx;
	terrainWidth_ = width;
	terrainHeight_ = height;

	HeightMap_ = new D3DXVECTOR3[terrainWidth_ * terrainHeight_];
	if (!HeightMap_) return false;
	*HeightMap_ = *heightMap;
	result = Grid.Initialize(d3dx, HeightMap_, terrainWidth_, terrainHeight_, _top, _bottom, Plain::Grid, Plain::SolidColor);
	result = Plot.Initialize(d3dx, HeightMap_, terrainWidth_, terrainHeight_, _top, _bottom, Plain::Solid, colorMode);
	
	if (!result) return false;
	return true;
}

bool TerrainClass::LoadHeightMap(string** header, string** data, LPWSTR filename)
{
	int error;
	unsigned long count;
	int imageSize, i, j, k, index;
	unsigned char height;

	fstream fin;
	char input;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	*header = new string[25];
	if (!*header) return false;

	*data = new string[25];
	if (!*data) return false;

	i = 0;
	k = 0;
	terrainWidth_ = 0;
	terrainHeight_ = 0;

	fin.get(input);
	while (input != '[')
	{
		fin.get(input);
	}
	while (true)
	{
		while (input != ']')
		{
			fin.get(input);
			if (input == '\n') continue;
			(*header)[i] += input;
		}
		(*header)[i].pop_back();
		if ((*header)[i].compare("Liczba Kolumn") == 0)
		{
			fin >> terrainWidth_;
			(*data)[i] = to_string(terrainWidth_);
		}
		if ((*header)[i].compare("Liczba Linii") == 0)
		{
			fin >> terrainHeight_;
			(*data)[i] = to_string(terrainHeight_);
		}
		if ((*header)[i].compare("Dane") == 0)
		{
			(*header)[i] = "";
			break;
		}

		while (input != '[')
		{
			fin.get(input);
			if (input == '\n') continue;
			(*data)[i] += input;
		}
		(*data)[i].pop_back();
		i++;
	}

	if (terrainWidth_ == 0) terrainWidth_ = terrainHeight_;
	HeightMap_ = new D3DXVECTOR3[terrainWidth_ * terrainHeight_];
	if (!HeightMap_)
	{
		return false;
	}

	_top = 0;
	_bottom = 0;
	for (j = 0; j<terrainHeight_; j++)
	{
		for (i = 0; i<terrainWidth_; i++)
		{

			index = (terrainHeight_ * j) + i;

			HeightMap_[index].x = (float)i;
			fin >> HeightMap_[index].y;
			HeightMap_[index].z = (float)j;
			fin.get(input);
			if (HeightMap_[index].y > _top) _top = HeightMap_[index].y;
			else if (HeightMap_[index].y < _bottom) _bottom = HeightMap_[index].y;

		}
	}
	fin.close();
	return true;
}

void TerrainClass::Shutdown()
{
	ShutdownHeightMap();
	return;
}

void TerrainClass::ShutdownHeightMap()
{
	if (HeightMap_)
	{
		delete[] HeightMap_;
		HeightMap_ = 0;
	}

	return;
}

bool TerrainClass::HighlightNode(POINT node)
{
	bool result;

	if (node == ClickPoint_) return true;

	if (node.y < terrainWidth_ &&
		node.x < terrainWidth_ &&
		node.x >= 0 &&
		node.y >= 0)
	{
		ClickPoint_ = node;
	}
	else
	{
		ClickPoint_ = { -1, -1 };
	}
	//result = Update(true, false, false);
		
	return true;
}

inline bool operator==(const POINT& p1, const POINT& p2)
{
	if (p1.x == p2.x && p1.y == p2.y) return true;
	else return false;
}

inline bool operator!=(const POINT& p1, const POINT& p2)
{
	return !(p1 == p2);
}