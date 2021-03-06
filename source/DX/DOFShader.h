// texture shader.h
#ifndef _DOFSHADER_H_
#define _DOFSHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class DOFShader: public BaseShader
{
public:

	DOFShader(ID3D11Device* device, HWND hwnd);
	~DOFShader();

	void SetShaderParameters( ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		ID3D11ShaderResourceView* scene, ID3D11ShaderResourceView* blur );
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);
	
private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif