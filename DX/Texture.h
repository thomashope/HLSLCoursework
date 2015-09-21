// texture.h
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <string>
#include <fstream>

using namespace DirectX;

class Texture
{
public:
	Texture(ID3D11Device* device, WCHAR* filename);
	~Texture();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool does_file_exist(const WCHAR *fileName);

	ID3D11ShaderResourceView* m_texture;
};

#endif