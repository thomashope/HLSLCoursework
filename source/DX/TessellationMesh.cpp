// Mesh.cpp
#include "tessellationmesh.h"

TessellationMesh::TessellationMesh(ID3D11Device* device, WCHAR* textureFilename)
{
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);

	// Load the texture for this model.
	LoadTexture(device, textureFilename);
}


TessellationMesh::~TessellationMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}



void TessellationMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;

	// Set the number of indices in the index array.
	m_indexCount = 24;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Load the vertex array with data.

	// front
	vertices[0].position = XMFLOAT3(-0.5f, 0.5f, -0.5f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[1].position = XMFLOAT3(0.5f, 0.5f, -0.5f);  // Top right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[2].position = XMFLOAT3(-0.5f, -0.5f, -0.5f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[3].position = XMFLOAT3(0.5f, -0.5f, -0.5f);  // Bottom right.
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// back
	vertices[4].position = XMFLOAT3( -0.5f, 0.5f, 0.5f );  // Top left.
	vertices[4].texture = XMFLOAT2( 0.0f, 1.0f );
	vertices[4].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	vertices[5].position = XMFLOAT3( 0.5f, 0.5f, 0.5f );  // Top right.
	vertices[5].texture = XMFLOAT2( 1.0f, 1.0f );
	vertices[5].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	vertices[6].position = XMFLOAT3( -0.5f, -0.5f, 0.5f );  // Bottom left.
	vertices[6].texture = XMFLOAT2( 0.0f, 0.0f );
	vertices[6].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );
	vertices[7].position = XMFLOAT3( 0.5f, -0.5f, 0.5f );  // Bottom right.
	vertices[7].texture = XMFLOAT2( 1.0f, 0.0f );
	vertices[7].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );

	// Load the index array with data.
	indices[0] = 0;  // front
	indices[1] = 1;  
	indices[2] = 2;  
	indices[3] = 3;

	indices[4] = 5; // back
	indices[5] = 4;
	indices[6] = 7;
	indices[7] = 6;
	
	indices[8] = 4; // top
	indices[9] = 5;
	indices[10] = 0;
	indices[11] = 1;

	indices[12] = 2; // bottom
	indices[13] = 3;
	indices[14] = 6;
	indices[15] = 7;

	indices[16] = 1; // right
	indices[17] = 5;
	indices[18] = 3;
	indices[19] = 7;

	indices[20] = 4; // left
	indices[21] = 0;
	indices[22] = 6;
	indices[23] = 2;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void TessellationMesh::SendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case control patch for tessellation.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
}

