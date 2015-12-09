// Light shader.h
// Basic single light shader setup
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "BaseShader.h"
#include "light.h"

using namespace std;
using namespace DirectX;


class ShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[2];
		XMMATRIX lightProjection[2];
	};

	struct PSLightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 position[2];
		XMFLOAT4 diffuse[2];

		// attenuation x, y, z, w == range, constant, linear, quadratic
		XMFLOAT4 attenuation[2];
	};

	struct VSLightBufferType
	{
		XMFLOAT3 position[2];
		XMFLOAT2 padding;
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* depthMap1, ID3D11ShaderResourceView* depthMap2, Light* lights[] );
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;

};

#endif