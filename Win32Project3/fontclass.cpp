///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"

FontClass::FontClass()
{
	Font_ = 0;
	_Texture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	std::ifstream fin;
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

void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (Font_)
	{
		delete[] Font_;
		Font_ = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	_Texture = new TextureClass;
	if (!_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = _Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (_Texture)
	{
		_Texture->Shutdown();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return _Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;
	float drawX = 0;

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, 0.0f, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + Font_[letter].size), -16, 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, -16, 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, 0.0f, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + Font_[letter].size, 0.0f, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + Font_[letter].size), -16, 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + Font_[letter].size + 1.0f;
		}
	}

	return;
}
