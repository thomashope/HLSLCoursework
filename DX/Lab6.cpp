// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "Lab6.h"
#include <iostream>

Lab6::Lab6( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh objects
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh( m_Direct3D->GetDevice(), L"../res/NUKAGE1.png" );
	m_OrthoMesh = new OrthoMesh( m_Direct3D->GetDevice(), 200, 150, -300, 225 );

	// Create alternate render targets
	m_RenderTexture = new RenderTexture( m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );

	// create she shaders
	m_JellyShader = new JellyShader( m_Direct3D->GetDevice(), hwnd );
	m_NormalShader = new PointLightShader( m_Direct3D->GetDevice(), hwnd );
	m_TextureShader = new TextureShader( m_Direct3D->GetDevice(), hwnd );

	// declare all the lights with default values
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		m_Lights[i] = new Light;
	}

	m_Lights[0]->SetPosition( 0.0f, 0.0f, 0.0f );
	m_Lights[0]->SetAmbientColour( 0.2f, 0.2f, 0.2f, 1.0f );
	m_Lights[0]->SetDiffuseColour( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Lights[0]->SetSpecularColour( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Lights[0]->SetRange( 4.0f );

	m_Lights[1]->SetPosition( 10.0f, 0.0f, 10.0f );
	m_Lights[1]->SetDiffuseColour( 0.0f, 1.0f, 0.0f, 1.0f );
	m_Lights[1]->SetSpecularColour( 0.0f, 1.0f, 0.0f, 1.0f );

	m_Lights[2]->SetPosition( 20.0f, 0.0f, 0.0f );
	m_Lights[2]->SetDiffuseColour( 1.0f, 1.0f, 0.0f, 1.0f );

	m_Lights[3]->SetPosition( 30.0f, -1.0f, 20.0f );
	m_Lights[3]->SetDiffuseColour( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Lights[3]->SetSpecularColour( 0.1f, 0.9f, 1.0f, 1.0f );
	m_Lights[3]->setAttenuation( 0.1f, 0.1f, 0.0f );
	m_Lights[3]->SetRange( 9.5f );
	m_Lights[3]->SetSpecularPower( 20.0f );
}

Lab6::~Lab6()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the meshs
	if( m_SphereMesh )
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}

	if( m_PlaneMesh )
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if( m_OrthoMesh )
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}

	// Release the shaders
	if( m_JellyShader )
	{
		delete m_JellyShader;
		m_JellyShader = 0;
	}

	if( m_NormalShader )
	{
		delete m_NormalShader;
		m_NormalShader = 0;
	}

	// Release the lights
	for( int i = 0; i < NUM_LIGHTS; i++ )
	{
		if( m_Lights[i] )
		{
			delete m_Lights[i];
			m_Lights[i] = 0;
		}
	}
}


bool Lab6::Frame()
{
	bool result;

	result = BaseApplication::Frame();
	if( !result )
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if( !result )
	{
		return false;
	}

	return true;
}

bool Lab6::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;


	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );

	m_time += m_Timer->GetTime();

	// render to texture
	{
		m_RenderTexture->SetRenderTarget( m_Direct3D->GetDeviceContext() );
		m_RenderTexture->ClearRenderTarget( m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f );

		// render sphere
		m_SphereMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture( ), m_Camera, m_Lights );
		m_NormalShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

		m_Direct3D->SetBackBufferRenderTarget();
	}

	// Clear the render targets
	m_Direct3D->BeginScene(0.1f, 0.1f, 0.2f, 1.0f);

	// move the lights
	{
		static float radians;
		float radius = 6.0f;
		radians += m_Timer->GetTime() * 1.6f;

		m_Lights[3]->SetPosition( 30.0f + sinf( radians ) * radius, -1.0f, 20.0f - cosf( radians ) * radius );
	}

	// move up
	worldMatrix = XMMatrixTranslation(0, 2, 0);

	// render sphere
	m_SphereMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Camera, m_Lights );
	m_NormalShader->Render( m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount() );

	// another far away sphere
	worldMatrix = XMMatrixTranslation( 20, 0, 20 );
	m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Camera, m_Lights );
	m_NormalShader->Render( m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount() );

	// move down
	worldMatrix = XMMatrixTranslation( -10, -2, -10 );

	// render plane
	m_PlaneMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_JellyShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Camera, m_time, m_Lights );
	m_JellyShader->Render( m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount() );
	
	m_Direct3D->GetWorldMatrix( worldMatrix );

	// render ortho plane
	m_Direct3D->TurnZBufferOff();

	m_OrthoMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView( ) );
	m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_OrthoMesh->GetIndexCount( ) );

	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}