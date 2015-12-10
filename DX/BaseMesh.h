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

protected:
	virtual void InitBuffers(ID3D11Device*) = 0;
	void LoadTexture(ID3D11Device*, WCHAR*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};

#endif