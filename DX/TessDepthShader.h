// Light shader.h
// Basic single light shader setup
#ifndef _TESSDEPTHSHADER_H_
#define _TESSDEPTHSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class TessDepthShader : public BaseShader
{
private:
	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 padding;
	};

	struct VertexManipBufferType
	{
		float time;
		XMFLOAT3 padding;
	};

public:

	TessDepthShader(ID3D11Device* device, HWND hwnd);
	~TessDepthShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float tesselationFactor, float time);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_tessellationBuffer;
	ID3D11Buffer* m_vertexManipBuffer;
};

#endif