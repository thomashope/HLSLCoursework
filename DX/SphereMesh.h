#ifndef _SPHEREMESH_H_
#define _SPHEREMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class SphereMesh : public BaseMesh
{

public:
	SphereMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 20);
	~SphereMesh();

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif