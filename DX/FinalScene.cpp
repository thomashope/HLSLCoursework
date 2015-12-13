// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "FinalScene.h"
#include <iostream>

Lab10::Lab10( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh objects
	m_FloorMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/floor_diffuse.png");
	m_FloorMesh->LoadNormalMap( m_Direct3D->GetDevice(), L"../res/floor2_normal.png" );
	m_FloorMesh->LoadSpecularMap( m_Direct3D->GetDevice(), L"../res/floor_specular.png" );

	m_Hellknight = new Model(m_Direct3D->GetDevice(), L"../res/hellknight_diffuse.png", L"../res/hellknight.obj");
	m_MagicSphere = new TessellationMesh( m_Direct3D->GetDevice(), L"" );

	m_FullscreenMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_TopLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, screenHeight / 4);
	m_TopRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, screenHeight / 4);
	m_BottomLeftMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, -screenWidth / 4, -screenHeight / 4);
	m_BottomRightMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, screenWidth / 4, -screenHeight / 4);

	// Create alternate render targets with meshes to render them on
	m_ShadowMap1 = new RenderTexture( m_Direct3D->GetDevice( ), 1536, 1536, SCREEN_NEAR, SCREEN_DEPTH );
	m_ShadowMap2 = new RenderTexture( m_Direct3D->GetDevice( ), 1536, 1536, SCREEN_NEAR, SCREEN_DEPTH );
	m_SceneLighting = new RenderTexture( m_Direct3D->GetDevice( ), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_BlobLighting = new RenderTexture( m_Direct3D->GetDevice( ), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_BlobNormals = new RenderTexture( m_Direct3D->GetDevice( ), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_BlendedScene = new RenderTexture( m_Direct3D->GetDevice( ), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH );
	m_BluredScene = new RenderTexture( m_Direct3D->GetDevice( ), screenWidth/4, screenHeight/4, SCREEN_NEAR, SCREEN_DEPTH );

	// create the shaders
	m_DepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_ShadowShader = new ShadowShader( m_Direct3D->GetDevice( ), hwnd );
	m_NormalShadowShader = new NormalShadowShader( m_Direct3D->GetDevice( ), hwnd );
	m_TessDepthShader = new TessDepthShader( m_Direct3D->GetDevice( ), hwnd );
	m_TessColourShader = new TessColourShader( m_Direct3D->GetDevice( ), hwnd );
	m_TessNormalShader = new TessNormalShader( m_Direct3D->GetDevice( ), hwnd );
	m_BlendShader = new BlendShader( m_Direct3D->GetDevice(), hwnd );
	m_ShowNormalShader = new ShowNormalShader( m_Direct3D->GetDevice( ), hwnd );
	m_BlurShader = new BoxBlurShader( m_Direct3D->GetDevice( ), hwnd, screenWidth, screenHeight );
	m_DOFShader = new DOFShader( m_Direct3D->GetDevice( ), hwnd );

	// declare all the lights with default values
	for (int i = 0; i < 2; i++)
	{
		m_Lights[i] = new Light;
	}

	m_Lights[0]->SetPosition( -7.0f, 4.0f, -4.0f );
	m_Lights[0]->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Lights[0]->SetAmbientColour( 0.1f, 0.1f, 0.1f, 1.0f );
	m_Lights[0]->SetDiffuseColour( 0.8f, 0.8f, 1.0f, 1.0f );
	m_Lights[0]->SetSpecularColour( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Lights[0]->SetRange( 40.0f );
	m_Lights[0]->setAttenuation(0.1f, 0.1f, 0.0f);

	
	m_Lights[1]->SetPosition( -6.0f, 3.5f, -6.0f );
	m_Lights[1]->SetLookAt( 0.0f, 0.0f, 0.0f );
	m_Lights[1]->SetDiffuseColour( 0.6f, 0.7f, 0.6f, 1.0f );
	m_Lights[1]->SetSpecularColour( 0.0f, 0.0f, 0.0f, 1.0f );
	m_Lights[1]->SetRange( 60.0f );
	m_Lights[1]->setAttenuation( 0.1f, 0.1f, 0.0f );

	m_showNormals = false;
	m_displayMode = 2;
	m_tesselation = 1.0f;
}

Lab10::~Lab10()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the meshs
	deleteIfNotNull<PlaneMesh>(m_FloorMesh);
	deleteIfNotNull<Model>( m_Hellknight );
	deleteIfNotNull<TessellationMesh>( m_MagicSphere );

	deleteIfNotNull<OrthoMesh>(m_FullscreenMesh);
	deleteIfNotNull<OrthoMesh>(m_TopLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_TopRightMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomLeftMesh);
	deleteIfNotNull<OrthoMesh>(m_BottomRightMesh);
	
	// release the textures
	deleteIfNotNull<RenderTexture>( m_ShadowMap1 );
	deleteIfNotNull<RenderTexture>( m_ShadowMap2 );
	deleteIfNotNull<RenderTexture>( m_SceneLighting );
	deleteIfNotNull<RenderTexture>( m_BlobLighting );
	deleteIfNotNull<RenderTexture>( m_BlobNormals );
	deleteIfNotNull<RenderTexture>( m_BlendedScene );
	deleteIfNotNull<RenderTexture>( m_BluredScene );
	
	// Release the shaders
	deleteIfNotNull<DepthShader>(m_DepthShader);
	deleteIfNotNull<ShadowShader>( m_ShadowShader );
	deleteIfNotNull<NormalShadowShader>( m_NormalShadowShader );
	deleteIfNotNull<TessDepthShader>( m_TessDepthShader );
	deleteIfNotNull<TessColourShader>( m_TessColourShader );
	deleteIfNotNull<TessNormalShader>( m_TessNormalShader );
	deleteIfNotNull<BlendShader>( m_BlendShader );
	deleteIfNotNull<DOFShader>( m_DOFShader );
	deleteIfNotNull<BoxBlurShader>( m_BlurShader );

	// Release the lights
	for( int i = 0; i < 2; i++ )
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

	/* Handle keyboard input */

	// Toggle normals
	if( m_Input->isKeyDown( VK_NUMPAD1 ) )
	{
		m_showNormals = !m_showNormals;
		m_Input->SetKeyUp( VK_NUMPAD1 );
	}

	// Switch display mode
	if( m_Input->isKeyDown( VK_NUMPAD4 ) )
	{
		m_displayMode = 0;
	}
	else if( m_Input->isKeyDown( VK_NUMPAD5 ) )
	{
		m_displayMode = 1;
	}
	else if( m_Input->isKeyDown( VK_NUMPAD6 ) )
	{
		m_displayMode = 2;
	}

	// adjust tesselation
	if( m_Input->isKeyDown( VK_NUMPAD2 ) )
	{
		m_tesselation += m_Timer->GetTime() * 4;
	}
	else if( m_Input->isKeyDown( VK_NUMPAD3 ) )
	{
		m_tesselation -= m_Timer->GetTime() * 4;
	}
	if( m_tesselation < 1.0f ) m_tesselation = 1.0f;
	else if( m_tesselation > 31.0f ) m_tesselation = 31.0f;
	
	// calculate the distortion frequency for the tesselated shape
	m_frequency.x = sin( m_time * 3.4f );
	m_frequency.y = sin( m_time * 5.3f );
	m_frequency.z = sin( m_time * 2.7f );
	m_frequency.w = 0.5f + sin( m_time * 2.0f ) * 0.3f;

	// move the lights
	{
		static float radians;
		float radius = 11;
		radians += m_Timer->GetTime( ) * 1.0f;

		m_Lights[1]->SetPosition(
			sin( radians ) * radius,
			m_Lights[1]->GetPosition( ).y,
			cos( radians ) * radius );

		m_Lights[1]->SetLookAt( 0.0f, 0.0f, 0.0f );
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
	m_Camera->Update();				// Generate the view matrix based on the camera's position.
	m_time += m_Timer->GetTime();	// Update the world time

	RenderShadowmap1();				// Shadow map for light 0
	RenderShadowmap2();				// shadow map for light 1
	RenderSceneLighting();			// scene with shadows applied
	RenderBlobLighting();			// render the blob with colour
	RenderBlobNormals();			// create the normal map for the blob
	BlendScene();					// blend the blob into the scene
	BlurScene();					// blur the blended scene

	if( m_displayMode == 2 )
	{
		ShowFinalScene();
	}
	else
	{
		ShowBuffers( );
	}

	return true;
}

void Lab10::RenderScene()
{
	// Get the world, view and projection matricies from the camera and Direct3D objects.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// set Render target to texture	
	m_SceneLighting->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_SceneLighting->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.1f, 0.1f, 0.1f, 1.0f );

	// render model
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation(0,2,0), XMMatrixScaling(0.3f, 0.3f, 0.3f) );
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Hellknight->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Hellknight->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Hellknight->GetIndexCount());

	// render blob
	worldMatrix = XMMatrixTranslation( 3.0f, 0.0f, 3.0f );
	m_MagicSphere->SendData( m_Direct3D->GetDeviceContext() );
	m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_MagicSphere->GetTexture( ) );
	m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_MagicSphere->GetIndexCount( ) );

	// render plane
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation( -40, -2, -40 ), XMMatrixScaling( 0.2f, 1.0f, 0.2f ) );
	m_FloorMesh->SendData(m_Direct3D->GetDeviceContext());
	//m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_FloorMesh->GetTexture());
	//m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FloorMesh->GetIndexCount());
}

void Lab10::RenderShadowmap1()
{
	// Get the world, view and projection matricies from the camera and Direct3D objects.
	XMMATRIX worldMatrix;
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Lights[0]->GenerateViewMatrix();
	XMMATRIX viewMatrix( m_Lights[0]->GetViewMatrix() );

	m_Lights[0]->GenerateProjectionMatrix(SCREEN_NEAR, m_Lights[0]->GetRange());
	XMMATRIX projectionMatrix( m_Lights[0]->GetProjectionMatrix() );

	// set Render target to texture	
	m_ShadowMap1->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_ShadowMap1->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.0f, 0.0f, 0.0f, 1.0f );
	
	// render model
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 2, 0), XMMatrixScaling(0.3f, 0.3f, 0.3f));;
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Hellknight->SendData( m_Direct3D->GetDeviceContext() );
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Hellknight->GetIndexCount());

	// render plane
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation( -40, -2, -40 ), XMMatrixScaling( 0.2f, 1.0f, 0.2f ) );
	m_FloorMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_FloorMesh->GetIndexCount(), true);

	// Only write che colour of the blob to the depth texture	
	m_Direct3D->TurnOnColourOnly();

	// render blob
	// cap tesselation for the shadow map at 16
	worldMatrix = XMMatrixTranslation( -2.0f, 0.0f, -2.0f );
	m_MagicSphere->SendData( m_Direct3D->GetDeviceContext() );
	m_TessDepthShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, ((m_tesselation < 16.0f) ? m_tesselation : 16.0f), m_frequency );
	m_TessDepthShader->Render( m_Direct3D->GetDeviceContext(), m_MagicSphere->GetIndexCount() );

	m_Direct3D->TurnOffColourOnly();
}

void Lab10::RenderShadowmap2()
{
	// Get the world, view and projection matricies from the camera and Direct3D objects.
	XMMATRIX worldMatrix;
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Lights[1]->GenerateViewMatrix( );
	XMMATRIX viewMatrix( m_Lights[1]->GetViewMatrix( ) );

	m_Lights[1]->GenerateProjectionMatrix( SCREEN_NEAR, m_Lights[1]->GetRange( ) );
	XMMATRIX projectionMatrix( m_Lights[1]->GetProjectionMatrix( ) );

	// set Render target to texture	
	m_ShadowMap2->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_ShadowMap2->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.0f, 0.0f, 0.0f, 1.0f );

	// render model
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 2, 0), XMMatrixScaling(0.3f, 0.3f, 0.3f));;
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Hellknight->SendData( m_Direct3D->GetDeviceContext( ) );
	m_DepthShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix );
	m_DepthShader->Render( m_Direct3D->GetDeviceContext( ), m_Hellknight->GetIndexCount( ) );

	// render plane
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation( -40, -2, -40 ), XMMatrixScaling( 0.2f, 1.0f, 0.2f ) );
	m_FloorMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_DepthShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix );
	m_DepthShader->Render( m_Direct3D->GetDeviceContext( ), m_FloorMesh->GetIndexCount( ) , true);

	// Only write che colour of the blob to the depth texture	
	m_Direct3D->TurnOnColourOnly( );

	// render blob
	// cap tesselation for the shadow map at 16
	worldMatrix = XMMatrixTranslation( -2.0f, 0.0f, -2.0f );
	m_MagicSphere->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TessDepthShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, ((m_tesselation < 16.0f) ? m_tesselation : 16.0f), m_frequency );
	m_TessDepthShader->Render( m_Direct3D->GetDeviceContext( ), m_MagicSphere->GetIndexCount( ) );
	m_Direct3D->TurnOffColourOnly( );
}

void Lab10::RenderSceneLighting()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// set Render target to texture	
	m_SceneLighting->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_SceneLighting->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.1f, 0.1f, 0.1f, 1.0f );

	// render model
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 2, 0), XMMatrixScaling(0.3f, 0.3f, 0.3f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, XM_PI * 0.5f, 0));
	m_Hellknight->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_Hellknight->GetTexture( ), m_ShadowMap1->GetShaderResourceView( ), m_ShadowMap2->GetShaderResourceView( ), m_Lights );
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_Hellknight->GetIndexCount());
	// Render the normals for the model
	if( m_showNormals )
	{
		m_ShowNormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix );
		m_ShowNormalShader->Render( m_Direct3D->GetDeviceContext( ), m_Hellknight->GetIndexCount( ) );
	}

	// render plane
	worldMatrix = XMMatrixMultiply( XMMatrixTranslation( -40, -2, -40 ), XMMatrixScaling( 0.2f, 1.0f, 0.2f ) );
	//worldMatrix = XMMatrixMultiply( worldMatrix, XMMatrixRotationY( 5.0f ) );
	m_FloorMesh->SendData(m_Direct3D->GetDeviceContext());
	m_NormalShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_FloorMesh->GetTexture( ), m_ShadowMap1->GetShaderResourceView( ), m_ShadowMap2->GetShaderResourceView(), m_FloorMesh->GetNormals(), m_Lights );
	m_NormalShadowShader->Render(m_Direct3D->GetDeviceContext(), m_FloorMesh->GetIndexCount());
}

void Lab10::RenderBlobLighting()
{
	// Get the world, view and projection matricies from the camera and Direct3D objects.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );

	m_BlobLighting->SetRenderTarget( m_Direct3D->GetDeviceContext() );
	m_BlobLighting->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.0f, 0.0f, 0.0f, 1.0f );

	// render the blob
	worldMatrix = XMMatrixTranslation( -2.0f, 0.0f, -2.0f );
	m_MagicSphere->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TessColourShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_MagicSphere->GetTexture( ), m_tesselation, m_frequency );
	m_TessColourShader->Render( m_Direct3D->GetDeviceContext( ), m_MagicSphere->GetIndexCount( ) );
}

void Lab10::RenderBlobNormals()
{
	// Get the world, view and projection matricies from the camera and Direct3D objects.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );

	m_BlobNormals->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_BlobNormals->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.0f, 0.0f, 0.0f, 1.0f );

	worldMatrix = XMMatrixTranslation( -2.0f, 0.0f, -2.0f );
	m_MagicSphere->SendData( m_Direct3D->GetDeviceContext( ) );
	m_TessNormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, m_tesselation, m_frequency );
	m_TessNormalShader->Render( m_Direct3D->GetDeviceContext( ), m_MagicSphere->GetIndexCount( ) );
}

void Lab10::BlendScene()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );

	m_BlendedScene->SetRenderTarget( m_Direct3D->GetDeviceContext() );
	m_BlendedScene->ClearRenderTarget( m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 0.0f );
	//m_Direct3D->SetBackBufferRenderTarget( );
	//m_Direct3D->ResetViewport( );
	//m_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	m_Direct3D->TurnZBufferOff();

	// Blend the textures together
	m_FullscreenMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_BlendShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix,
		m_SceneLighting->GetShaderResourceView(), m_BlobLighting->GetShaderResourceView(), m_BlobNormals->GetShaderResourceView() );
	m_BlendShader->Render( m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount() );

	m_Direct3D->TurnZBufferOn( );
	//m_Direct3D->EndScene( );
}

void Lab10::BlurScene()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );

	m_BluredScene->SetRenderTarget( m_Direct3D->GetDeviceContext( ) );
	m_BluredScene->ClearRenderTarget( m_Direct3D->GetDeviceContext( ), 0.0f, 0.0f, 0.0f, 0.0f );
	//m_Direct3D->SetBackBufferRenderTarget( );
	//m_Direct3D->ResetViewport( );
	//m_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	m_Direct3D->TurnZBufferOff( );

	// Blend the textures together
	m_FullscreenMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_BlurShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlendedScene->GetShaderResourceView( ) );
	m_BlurShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

	m_Direct3D->TurnZBufferOn( );
	//m_Direct3D->EndScene( );
}

void Lab10::ShowFinalScene( )
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );

	m_Direct3D->SetBackBufferRenderTarget( );
	m_Direct3D->ResetViewport( );
	m_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	m_Direct3D->TurnZBufferOff( );

	// Blend the textures together
	m_FullscreenMesh->SendData( m_Direct3D->GetDeviceContext( ) );
	m_DOFShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlendedScene->GetShaderResourceView( ), m_BluredScene->GetShaderResourceView() );
	m_DOFShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

	m_Direct3D->TurnZBufferOn( );

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene( );
}

void Lab10::ShowBuffers( )
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Direct3D->GetOrthoMatrix( orthoMatrix );
	m_Camera->GetBaseViewMatrix( baseViewMatrix );
	
	// Set render target to back buffer
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();	
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->TurnZBufferOff();
	
	if( m_displayMode == 0 )
	{
		m_TopLeftMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_ShadowMap1->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

		m_TopRightMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_ShadowMap2->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

		m_BottomLeftMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlobLighting->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

		m_BottomRightMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_SceneLighting->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );
	}
	else
	{
		m_TopLeftMesh->SendData(m_Direct3D->GetDeviceContext());
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlobLighting->GetShaderResourceView( ) );
		m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());
	
		m_TopRightMesh->SendData(m_Direct3D->GetDeviceContext());
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlobNormals->GetShaderResourceView( ) );
		m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullscreenMesh->GetIndexCount());

		m_BottomLeftMesh->SendData(m_Direct3D->GetDeviceContext());
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BlendedScene->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );

		m_BottomRightMesh->SendData( m_Direct3D->GetDeviceContext( ) );
		m_TextureShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, baseViewMatrix, orthoMatrix, m_BluredScene->GetShaderResourceView( ) );
		m_TextureShader->Render( m_Direct3D->GetDeviceContext( ), m_FullscreenMesh->GetIndexCount( ) );
	}
	

	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene( );
}