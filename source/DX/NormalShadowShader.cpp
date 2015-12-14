// texture shader.cpp
#include "NormalShadowShader.h"


NormalShadowShader::NormalShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"../shaders/normalshadow_vs.hlsl", L"../shaders/normalshadow_ps.hlsl");
}


NormalShadowShader::~NormalShadowShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

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

	// Release the light constant buffer.
	if (m_LightBuffer)
	{
		m_LightBuffer->Release();
		m_LightBuffer = 0;
	}
	// Release the light constant buffer.
	if (m_LightBuffer2)
	{
		m_LightBuffer2->Release();
		m_LightBuffer2 = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void NormalShadowShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc2;

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

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);
	
	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(PSLightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_LightBuffer);

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(VSLightBufferType);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc2, NULL, &m_LightBuffer2);

}


void NormalShadowShader::SetShaderParameters( ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap1, ID3D11ShaderResourceView* depthMap2, ID3D11ShaderResourceView* normal, Light* lights[] )
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	PSLightBufferType* PSlightPtr;
	VSLightBufferType* VSlightPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	XMMATRIX tLightViewMatrix[2];
	XMMATRIX tLightProjectionMatrix[2];


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	tLightViewMatrix[0] = XMMatrixTranspose(lights[0]->GetViewMatrix());
	tLightProjectionMatrix[0] = XMMatrixTranspose( lights[0]->GetProjectionMatrix( ) );
	tLightViewMatrix[1] = XMMatrixTranspose( lights[1]->GetViewMatrix( ) );
	tLightProjectionMatrix[1] = XMMatrixTranspose( lights[1]->GetProjectionMatrix( ) );

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	dataPtr->lightView[0] = tLightViewMatrix[0];
	dataPtr->lightProjection[0] = tLightProjectionMatrix[0];
	dataPtr->lightView[1] = tLightViewMatrix[1];
	dataPtr->lightProjection[1] = tLightProjectionMatrix[1];

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	
	// Send light data to pixel shader
	deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	PSlightPtr = (PSLightBufferType*)mappedResource.pData;
	// Send colour and attenuation for first light
	PSlightPtr->ambient[0]			= lights[0]->GetAmbientColour( );
	PSlightPtr->position[0]			= lights[0]->GetPosition4( );
	PSlightPtr->diffuse[0]			= lights[0]->GetDiffuseColour( );
	PSlightPtr->attenuation[0].x	= lights[0]->GetRange( );
	PSlightPtr->attenuation[0].y	= lights[0]->GetConstantAttenuation( );
	PSlightPtr->attenuation[0].z	= lights[0]->GetLinearAttenuation( );
	PSlightPtr->attenuation[0].w	= lights[0]->GetQuadraticAttenuation( );
	// Send colour and attenuation for second light
	PSlightPtr->ambient[1]			= lights[1]->GetAmbientColour( );
	PSlightPtr->position[1]			= lights[1]->GetPosition4( );
	PSlightPtr->diffuse[1]			= lights[1]->GetDiffuseColour( );
	PSlightPtr->attenuation[1].x	= lights[1]->GetRange( );
	PSlightPtr->attenuation[1].y	= lights[1]->GetConstantAttenuation( );
	PSlightPtr->attenuation[1].z	= lights[1]->GetLinearAttenuation( );
	PSlightPtr->attenuation[1].w	= lights[1]->GetQuadraticAttenuation( );

	deviceContext->Unmap(m_LightBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer);

	// Send light data to vertex shader
	deviceContext->Map(m_LightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	VSlightPtr = (VSLightBufferType*)mappedResource.pData;
	VSlightPtr->position[0]			= lights[0]->GetPosition4();
	VSlightPtr->position[1]			= lights[1]->GetPosition4();
	deviceContext->Unmap(m_LightBuffer2, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer2);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	// Set shader depth map texture resource.
	deviceContext->PSSetShaderResources( 1, 1, &depthMap1 );
	deviceContext->PSSetShaderResources( 2, 1, &depthMap2 );
	// Send the normal map
	deviceContext->PSSetShaderResources( 3, 1, &normal );
}

void NormalShadowShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->PSSetSamplers(1, 1, &m_sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}



