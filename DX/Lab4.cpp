// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "lab4.h"
#include <iostream>

Lab4::Lab4( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh object
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh( m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png" );

	// create she shaders
	m_LightShader = new PointLightShader( m_Direct3D->GetDevice(), hwnd );

	// declare all the lights with default values
	for (int i = 0; i < NUM_LIGHTS; i++) {
		m_Light[i] = new Light;
	}

	m_Light[0]->SetPosition( 0.0f, 0.0f, 0.0f );
	m_Light[0]->SetDiffuseColour( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Light[0]->SetRange( 4.0f );

	m_Light[1]->SetPosition( 10.0f, 0.0f, 10.0f );
	m_Light[1]->SetDiffuseColour( 0.0f, 1.0f, 0.0f, 1.0f );
}


Lab4::~Lab4()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}

	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}


	for (int i = 0; i < NUM_LIGHTS; i++) {
		if (m_Light[i])
		{
			delete m_Light[i];
			m_Light[i] = 0;
		}
	}
}


bool Lab4::Frame()
{
	bool result;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Lab4::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// move up
	worldMatrix = XMMatrixTranslation(0, 2, 0);

	// render sphere
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_LightShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
										m_SphereMesh->GetTexture(), m_Camera, m_Light );
	m_LightShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

	// another far away sphe
	worldMatrix = XMMatrixTranslation( 20, 0, 20 );
	m_LightShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix,
										m_SphereMesh->GetTexture( ), m_Camera, m_Light );
	m_LightShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

	// move down
	worldMatrix = XMMatrixTranslation( -10, -2, -10 );

	// render plane
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_LightShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
										m_PlaneMesh->GetTexture(), m_Camera, m_Light );
	m_LightShader->Render( m_Direct3D->GetDeviceContext( ), m_PlaneMesh->GetIndexCount( ) );


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}