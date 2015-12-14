// depth shader.cpp
#include "depthshader.h"


DepthShader::DepthShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"../shaders/depth_vs.hlsl", L"../shaders/depth_ps.hlsl");
	
	loadVertexShader( L"../shaders/depthnormal_vs.hlsl", m_vertexNormalShader);
}


DepthShader::~DepthShader()
{
	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void DepthShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

}


void DepthShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}

void DepthShader::Render( ID3D11DeviceContext* deviceContext, int indexCount, bool hasNormalData )
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout );

	// Set the vertex and pixel shaders that will be used to render.
	if( hasNormalData )
	{
		// The mesh that has been sent includes binormal and tangent data, so we need to use a vertex shader that takes account of that
	}
	else
	{
		deviceContext->VSSetShader( m_vertexShader, NULL, 0 );
	}
	deviceContext->PSSetShader( m_pixelShader, NULL, 0 );

	// if Hull shader is not null then set HS and DS
	if( m_hullShader )
	{
		deviceContext->HSSetShader( m_hullShader, NULL, 0 );
		deviceContext->DSSetShader( m_domainShader, NULL, 0 );
	}
	else
	{
		deviceContext->HSSetShader( NULL, NULL, 0 );
		deviceContext->DSSetShader( NULL, NULL, 0 );
	}

	// if geometry shader is not null then set GS
	if( m_geometryShader )
	{
		deviceContext->GSSetShader( m_geometryShader, NULL, 0 );
	}
	else
	{
		deviceContext->GSSetShader( NULL, NULL, 0 );
	}

	// Render the triangle.
	deviceContext->DrawIndexed( indexCount, 0, 0 );
}