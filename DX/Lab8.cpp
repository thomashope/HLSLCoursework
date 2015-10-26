// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "Lab8.h"
#include <iostream>

Lab8::Lab8( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh objects
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh( m_Direct3D->GetDevice(), L"../res/NUKAGE1.png" );

	// Create alternate render targets with meshes to render them on
	m_StandardSceneTexture = new RenderTexture( m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_StandardSceneMesh = new OrthoMesh( m_Direct3D->GetDevice(), screenWidth/2, screenHeight/2, -screenWidth/4, screenHeight/4 );
	m_BlurredSceneTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_BlurredSceneMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, screenHeight / 4);
	

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

Lab8::~Lab8()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the meshs
	deleteIfNotNull<SphereMesh>(m_SphereMesh);
	deleteIfNotNull<PlaneMesh>(m_PlaneMesh);

	deleteIfNotNull<OrthoMesh>(m_StandardSceneMesh);
	deleteIfNotNull<OrthoMesh>(m_BlurredSceneMesh);
	
	// release the textures
	deleteIfNotNull<RenderTexture>(m_StandardSceneTexture);
	deleteIfNotNull<RenderTexture>(m_BlurredSceneTexture);
	
	// Release the shaders
	deleteIfNotNull<JellyShader>(m_JellyShader);
	deleteIfNotNull<PointLightShader>(m_NormalShader);
	deleteIfNotNull<TextureShader>(m_TextureShader);

	// Release the lights
	for( int i = 0; i < NUM_LIGHTS; i++ )
	{
		deleteIfNotNull<Light>(m_Lights[i]);
	}
}


bool Lab8::Frame()
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

bool Lab8::Render()
{
	// Generate the view matrix based on the camera's position.
	m_Camera->Update();
	// update the world time
	m_time += m_Timer->GetTime();
	
	RenderToTexture();

	RenderToBackBuffer();

	return true;
}

void Lab8::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );

	// move the lights
	{
		static float radians;
		float radius = 6.0f;
		radians += m_Timer->GetTime() * 1.6f;

		m_Lights[3]->SetPosition(30.0f + sinf(radians) * radius, -1.0f, 20.0f - cosf(radians) * radius);
	}

	// set Render target to texture	
	m_StandardSceneTexture->SetRenderTarget( m_Direct3D->GetDeviceContext() );
	m_StandardSceneTexture->ClearRenderTarget( m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f );

	// move up
	worldMatrix = XMMatrixTranslation( 0, 2, 0 );

	// render sphere
	m_SphereMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture() );
	m_TextureShader->Render( m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount() );

	// another far away sphere
	worldMatrix = XMMatrixTranslation( 20, 0, 20 );
	m_NormalShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Camera, m_Lights);
	m_NormalShader->Render( m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount() );

	// move down
	worldMatrix = XMMatrixTranslation( -10, -2, -10 );

	// render wobbly plane
	m_PlaneMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_JellyShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture( ), m_Camera, m_time, m_Lights );
	m_JellyShader->Render( m_Direct3D->GetDeviceContext( ), m_PlaneMesh->GetIndexCount( ) );
	
	m_Direct3D->SetBackBufferRenderTarget();	
}

void Lab8::RenderToBackBuffer()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );
	
	// Set render target to back buffer
	m_Direct3D->SetBackBufferRenderTarget();
	
	/*
	// move the lights
	{
		static float radians;
		float radius = 6.0f;
		radians += m_Timer->GetTime( ) * 1.6f;

		m_Lights[3]->SetPosition( 30.0f + sinf( radians ) * radius, -1.0f, 20.0f - cosf( radians ) * radius );
	}

	// Render to back buffer
	m_Direct3D->BeginScene( 0.1f, 0.1f, 0.2f, 1.0f );

	// move up
	worldMatrix = XMMatrixTranslation( 0, 2, 0 );

	// render sphere
	m_SphereMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture( ), m_Camera, m_Lights );
	m_NormalShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

	// another far away sphere
	worldMatrix = XMMatrixTranslation( 20, 0, 20 );
	m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture( ), m_Camera, m_Lights );
	m_NormalShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

	// move down
	worldMatrix = XMMatrixTranslation( -10, -2, -10 );

	// render plane
	m_PlaneMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_JellyShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture( ), m_Camera, m_time, m_Lights );
	m_JellyShader->Render( m_Direct3D->GetDeviceContext( ), m_PlaneMesh->GetIndexCount( ) );
	*/

	// reset the world matrix
	m_Direct3D->GetWorldMatrix( worldMatrix );
	
	// render ortho planes
	m_Direct3D->TurnZBufferOff( );

	// render the scene normally in the top right
	m_StandardSceneMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_StandardSceneTexture->GetShaderResourceView( ) );
	m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_StandardSceneMesh->GetIndexCount( ) );

	// blurred scene in top left
	m_BlurredSceneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_StandardSceneTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_StandardSceneMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn( );

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene( );
}