#ifndef _PLANEMESH_H_
#define _PLANEMESH_H_

#include "BaseMesh.h"

class PlaneMesh : public BaseMesh
{

	struct NormalVertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 binormal;
		XMFLOAT3 tangent;
	};

public:
	PlaneMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 100);
	~PlaneMesh();

	void SendData( ID3D11DeviceContext* deviceContext ) override;

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif