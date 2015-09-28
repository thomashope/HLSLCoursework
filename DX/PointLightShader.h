// Light shader.h
// Basic single light shader setup
#ifndef _POINT_LIGHTSHADER_H_
#define _POINT_LIGHTSHADER_H_

#include "BaseShader.h"
#include "light.h"
#include "Camera.h"

using namespace std;
using namespace DirectX;


class PointLightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT3 position;
		float specularPower;
		XMFLOAT4 specular;
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

public:

	PointLightShader(ID3D11Device* device, HWND hwnd);
	~PointLightShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Camera* camera, Light* light);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
};

#endif