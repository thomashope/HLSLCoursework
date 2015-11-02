// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "Lab8.h"
#include <iostream>

Lab8::Lab8( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh objects
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh( m_Direct3D->GetDevice(), L"../res/NUKAGE1.png" );

	m_FullscreenMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_TopLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, screenHeight / 4);
	m_TopRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, screenHeight / 4);
	m_BottomLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, -screenHeight / 4);
	m_BottomRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, -screenHeight / 4);

	// Create alternate render targets with meshes to render them on
	m_StandardSceneTexture = new RenderTexture( m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_BlurredSceneTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_DownSampledSceneTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth / 4, screenHeight / 4, SCREEN_NEAR, SCREEN_DEPTH);

	m_HorizontalyBlurredTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_GuassianBlurTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// create the shaders
	m_JellyShader = new JellyShader( m_Direct3D->GetDevice(), hwnd );
	m_NormalShader = new PointLightShader( m_Direct3D->GetDevice(), hwnd );
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_BoxBlurShader = new BoxBlurShader(m_Direct3D->GetDevice(), hwnd, screenWidth, screenHeight);
	m_horizontalBlurShader = new HorizontalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_verticalBlurShader = new VerticalBlurShader(m_Direct3D->GetDevice(), hwnd);

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

	deleteIfNotNull<OrthoMesh>(m_FullscreenMesh);
	deleteIfNotNull<OrthoMesh>(m_TopLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_TopRightMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomRightMesh);
	
	// release the textures
	deleteIfNotNull<RenderTexture>(m_StandardSceneTexture);
	deleteIfNotNull<RenderTexture>(m_BlurredSceneTexture);
	deleteIfNotNull<RenderTexture>(m_DownSampledSceneTexture);
	deleteIfNotNull<RenderTexture>(m_HorizontalyBlurredTexture);
	deleteIfNotNull<RenderTexture>(m_GuassianBlurTexture);
	
	// Release the shaders
	deleteIfNotNull<JellyShader>(m_JellyShader);
	deleteIfNotNull<PointLightShader>(m_NormalShader);
	deleteIfNotNull<TextureShader>(m_TextureShader);
	deleteIfNotNull<HorizontalBlurShader>(m_horizontalBlurShader);
	deleteIfNotNull<VerticalBlurShader>(m_verticalBlurShader);

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
	
	RenderScene();
	
	RenderBoxBlurredScene();

	RenderHorizontalBlur();

	RenderGaussianBlur();

	RenderDownsampledScene();

	RenderToBackBuffer();

	return true;
}

void Lab8::RenderScene()
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

void Lab8::RenderBoxBlurredScene()
{
	XMMATRIX worldMatrix, viewMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	
	// set Render target to texture	
	m_BlurredSceneTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_BlurredSceneTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	m_Direct3D->TurnZBufferOff();
	
	m_FullscreenMesh->SendData(m_Direct3D->GetDeviceContext());
	m_BoxBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_StandardSceneTexture->GetShaderResourceView());
	m_BoxBlurShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();
}

void Lab8::RenderDownsampledScene()
{
	XMMATRIX worldMatrix, viewMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// set Render target to texture	
	m_DownSampledSceneTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_DownSampledSceneTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	m_Direct3D->TurnZBufferOff();

	m_FullscreenMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_StandardSceneTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();
}

void Lab8::RenderHorizontalBlur()
{
	XMMATRIX worldMatrix, viewMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// set Render target to texture	
	m_HorizontalyBlurredTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_HorizontalyBlurredTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	m_Direct3D->TurnZBufferOff();

	m_FullscreenMesh->SendData(m_Direct3D->GetDeviceContext());
	m_horizontalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix,
												m_DownSampledSceneTexture->GetShaderResourceView(), 800);

	m_horizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();
}

void Lab8::RenderGaussianBlur()
{
	XMMATRIX worldMatrix, viewMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// set Render target to texture	
	m_GuassianBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_GuassianBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	m_Direct3D->TurnZBufferOff();

	m_FullscreenMesh->SendData(m_Direct3D->GetDeviceContext());
	m_verticalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix,
												m_HorizontalyBlurredTexture->GetShaderResourceView(), 800);

	m_verticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();
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
	m_Direct3D->ResetViewport();
	
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->TurnZBufferOff();
	
	m_TopLeftMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_StandardSceneTexture->GetShaderResourceView( ) );
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_TopLeftMesh->GetIndexCount());

	m_TopRightMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_BlurredSceneTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_TopRightMesh->GetIndexCount());

	m_BottomLeftMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_DownSampledSceneTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_BottomLeftMesh->GetIndexCount());
	
	m_BottomRightMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_GuassianBlurTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_BottomRightMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene( );
}