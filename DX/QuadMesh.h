#ifndef QUADMESH_H
#define QUADMESH_H

#include "BaseMesh.h"

using namespace DirectX;

class QuadMesh : public BaseMesh
{

public:
	QuadMesh(ID3D11Device* device, WCHAR* textureFilename);
	~QuadMesh();

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif