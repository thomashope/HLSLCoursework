// Light shader.h
// Geometry shader example.
#ifndef _GEOMETRYSHADER_H_
#define _GEOMETRYSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;


class GeometryShader : public BaseShader
{

public:

	GeometryShader(ID3D11Device* device, HWND hwnd);
	~GeometryShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif