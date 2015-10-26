#ifndef _BOXBLURSHADER_H
#define _BOXBLURSHADER_H

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class BoxBlurShader : public BaseShader
{
private:

	struct ScreenBufferType
	{
		float screenWidth;
		float screenHeight;
		XMFLOAT2 padding;
	};

public:

	BoxBlurShader(ID3D11Device* device, HWND hwnd, float width, float height);
	~BoxBlurShader();
	
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

	float screenWidth, screenHeight;

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_ScreenSizeBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif