// Light shader.h
// Geometry shader example.
#ifndef _SHOWNORMALSHADER_H_
#define _SHOWNORMALSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class ShowNormalShader : public BaseShader
{

public:

	ShowNormalShader(ID3D11Device* device, HWND hwnd);
	~ShowNormalShader();

	void SetShaderParameters( ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif