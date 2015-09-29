// Light shader.h
// Basic single light shader setup
#ifndef _POINT_LIGHTSHADER_H_
#define _POINT_LIGHTSHADER_H_

#include "BaseShader.h"
#include "light.h"
#include "Camera.h"


using namespace std;
using namespace DirectX;

// the number of lights the shader will handle before it breaks down and dies
#define NUM_LIGHTS 4

class PointLightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		// Ambient colour is global and defined by light[0]
		XMFLOAT4 diffuse[NUM_LIGHTS];
		XMFLOAT4 position[NUM_LIGHTS];
		XMFLOAT4 specular[NUM_LIGHTS];
		XMFLOAT4 ambient;

		// attenuation x, y, z, w == range, constant, linear, quadratic
		XMFLOAT4 attenuation[NUM_LIGHTS];
		// TODO turn below variables back into floats
		// have to be float4 due to some strange allignment issue
		//XMFLOAT4 range[NUM_LIGHTS];
		//XMFLOAT4 constAttenuation[NUM_LIGHTS];
		//XMFLOAT4 linearAttenuation[NUM_LIGHTS];
		//XMFLOAT4 quadraticAttenuation[NUM_LIGHTS];

		XMFLOAT4 specularPower[NUM_LIGHTS];
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

public:

	PointLightShader(ID3D11Device* device, HWND hwnd);
	~PointLightShader();

	void SetShaderParameters( ID3D11DeviceContext* deviceContext,
							  const XMMATRIX &world,
							  const XMMATRIX &view,
							  const XMMATRIX &projection,
							  ID3D11ShaderResourceView* texture,
							  Camera* camera,
							  Light* light[2]);

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