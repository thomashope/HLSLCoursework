// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "Lab10.h"
#include <iostream>

Lab10::Lab10( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh objects
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/NUKAGE1.png");
	m_Teapot = new Model(m_Direct3D->GetDevice(), L"../res/hellknight.png", L"../res/hellknight.obj");

	m_FullscreenMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_TopLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, screenHeight / 4);
	m_TopRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, screenHeight / 4);
	m_BottomLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, -screenHeight / 4);
	m_BottomRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, -screenHeight / 4);

	// Create alternate render targets with meshes to render them on
	m_SceneDepth = new RenderTexture(m_Direct3D->GetDevice(), 2048, 2048, SCREEN_NEAR, SCREEN_DEPTH);
	m_Scene = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_SceneWithShadows = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// create the shaders
	m_DepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);

	// declare all the lights with default values
	for (int i = 0; i < 1; i++)
	{
		m_Lights[i] = new Light;
	}

	m_Lights[0]->SetPosition( -5.0f, 5.0f, -5.0f );
	m_Lights[0]->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Lights[0]->SetAmbientColour( 0.2f, 0.2f, 0.2f, 1.0f );
	m_Lights[0]->SetDiffuseColour( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Lights[0]->SetSpecularColour( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Lights[0]->SetRange( 4.0f );

	/*
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
	*/
}

Lab10::~Lab10()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the meshs
	deleteIfNotNull<SphereMesh>(m_SphereMesh);
	deleteIfNotNull<PlaneMesh>(m_PlaneMesh);
	deleteIfNotNull<Model>(m_Teapot);

	deleteIfNotNull<OrthoMesh>(m_FullscreenMesh);
	deleteIfNotNull<OrthoMesh>(m_TopLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_TopRightMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomRightMesh);
	
	// release the textures
	deleteIfNotNull<RenderTexture>(m_SceneDepth);
	deleteIfNotNull<RenderTexture>(m_Scene);
	deleteIfNotNull<RenderTexture>(m_SceneWithShadows);
	
	// Release the shaders
	deleteIfNotNull<DepthShader>(m_DepthShader);
	deleteIfNotNull<ShadowShader>(m_ShadowShader);

	// Release the lights
	for( int i = 0; i < 1; i++ )
	{
		deleteIfNotNull<Light>(m_Lights[i]);
	}
}

bool Lab10::Frame()
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

bool Lab10::Render()
{
	// Generate the view matrix based on the camera's position.
	m_Camera->Update();
	// update the world time
	m_time += m_Timer->GetTime();
	
	RenderScene();

	RenderSceneDepthOnly();

	RenderSceneWithShadows();

	ShowScene();

	return true;
}

void Lab10::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// set Render target to texture	
	m_Scene->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_Scene->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	// render model
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation(0,2,0), XMMatrixScaling(0.3f, 0.3f, 0.3f) );
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Teapot->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Teapot->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Teapot->GetIndexCount());

	// another far away sphere
	worldMatrix = XMMatrixTranslation(20, 0, 20);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());

	// render plane
	worldMatrix = XMMatrixTranslation(-10, -2, -10);
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	m_Direct3D->SetBackBufferRenderTarget();
}

void Lab10::RenderSceneDepthOnly()
{
	XMMATRIX worldMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Lights[0]->GenerateViewMatrix();
	m_Lights[0]->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	XMMATRIX viewMatrix( m_Lights[0]->GetViewMatrix() );
	XMMATRIX projectionMatrix( m_Lights[0]->GetProjectionMatrix() );

	// set Render target to texture	
	m_SceneDepth->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_SceneDepth->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	// render model
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 2, 0), XMMatrixScaling(0.3f, 0.3f, 0.3f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Teapot->SendData( m_Direct3D->GetDeviceContext() );
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Teapot->GetIndexCount());

	// another far away sphere
	worldMatrix = XMMatrixTranslation(20, 0, 20);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());

	// render plane
	worldMatrix = XMMatrixTranslation( -10, -2, -10 );
	m_PlaneMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	m_Direct3D->SetBackBufferRenderTarget();	
}

void Lab10::RenderSceneWithShadows()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// set Render target to texture	
	m_SceneWithShadows->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_SceneWithShadows->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.1f, 0.1f, 0.1f, 1.0f);

	// render model
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 2, 0), XMMatrixScaling(0.3f, 0.3f, 0.3f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Teapot->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_Teapot->GetTexture(), m_SceneDepth->GetShaderResourceView(), m_Lights[0]);

	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_Teapot->GetIndexCount());

	// another far away sphere
	worldMatrix = XMMatrixTranslation(20, 0, 20);
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_SphereMesh->GetTexture(), m_SceneDepth->GetShaderResourceView(), m_Lights[0]);

	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());

	// render plane
	worldMatrix = XMMatrixTranslation(-10, -2, -10);
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_PlaneMesh->GetTexture(), m_SceneDepth->GetShaderResourceView(), m_Lights[0]);

	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	m_Direct3D->SetBackBufferRenderTarget();
}

void Lab10::ShowScene()
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
	
	m_TopLeftMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_Scene->GetShaderResourceView( ) );
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());
	
	m_TopRightMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_SceneDepth->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_BottomLeftMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_SceneWithShadows->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene( );
}