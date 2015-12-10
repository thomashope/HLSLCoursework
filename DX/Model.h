#ifndef _MODEL_H_
#define _MODEL_H_

#include "BaseMesh.h"
#include "TokenStream.h"
#include <vector>

using namespace DirectX;

class Model : public BaseMesh
{
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct NormalVertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	Model(ID3D11Device* device, WCHAR* textureFilename, WCHAR* filename);
	~Model();

	void SendData( ID3D11DeviceContext* deviceContext ) override;

protected:
	void InitBuffers(ID3D11Device* device);
	void LoadModel(WCHAR* filename);
	
	ModelType* m_model;	
};

#endif