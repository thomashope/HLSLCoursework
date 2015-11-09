// Colour shader.h
// Simple shader example.
#ifndef _DEPTHSHADER_H_
#define _DEPTHSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class DepthShader : public BaseShader
{

public:

	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection);
	//void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
};

#endif