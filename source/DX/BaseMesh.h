// Mesh.h
#ifndef _BASEMESH_H_
#define _BASEMESH_H_

#include <d3d11.h>
#include <directxmath.h>
#include "texture.h"

// uncomment to ask meshes to build and send the tangent and binormal data for use with bump mapping
//#define INCLUDE_NORMALDATA

using namespace DirectX;

class BaseMesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;

		// FOR NORMAL MAPPING
#ifdef INCLUDE_NORMALDATA
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
#endif
	};

public:
	BaseMesh();
	~BaseMesh();

	virtual void SendData( ID3D11DeviceContext* );
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void LoadNormalMap( ID3D11Device* device, WCHAR* filename ) { m_Normals = new Texture( device, filename ); }
	void LoadSpecularMap( ID3D11Device* device, WCHAR* filename ) { m_Specular = new Texture( device, filename ); }
	ID3D11ShaderResourceView* GetNormals( ) { return m_Normals->GetTexture( ); }
	ID3D11ShaderResourceView* GetSpecular( ) { return m_Specular->GetTexture( ); }

protected:
	virtual void InitBuffers(ID3D11Device*) = 0;
	void LoadTexture(ID3D11Device*, WCHAR*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	Texture* m_Normals;
	Texture* m_Specular;
};

#endif