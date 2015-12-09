// Light shader.h
// Basic single light shader setup
#ifndef _TESSCOLOURSHADER_H_
#define _TESSCOLOURSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class TessColourShader : public BaseShader
{
private:
	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 padding;
	};

	struct VertexManipBufferType
	{
		XMFLOAT4 frequency;
	};

public:

	TessColourShader(ID3D11Device* device, HWND hwnd);
	~TessColourShader();

	void SetShaderParameters( ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float tesselationFactor, XMFLOAT4 frequency );
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