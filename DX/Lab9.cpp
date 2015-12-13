// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "Lab9.h"
#include <iostream>

Lab9::Lab9( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in ) : BaseApplication( hinstance, hwnd, screenWidth, screenHeight, in )
{
	// Create Mesh object
	m_SphereMesh = new SphereMesh( m_Direct3D->GetDevice(), L"../res/brick1.dds" );
	m_PlaneMesh = new PlaneMesh( m_Direct3D->GetDevice(), L"../res/NUKAGE1.png" );
	m_TesselationMesh = new TessellationMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	// create she shaders
	m_JellyShader = new JellyShader(m_Direct3D->GetDevice(), hwnd);
	m_NormalShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
	m_TesselationShader = new TessDepthShader( m_Direct3D->GetDevice( ), hwnd );

	// declare all the lights with default values
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		m_Lights[i] = new Light;
	}

	tesselationFactor = 8.0f;

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
	m_Lights[3]->setAttenuation( 0.5f, 0.2f, 0.0f );
	m_Lights[3]->SetRange( 9.5f );
	m_Lights[3]->SetSpecularPower( 20.0f );
}


Lab9::~Lab9()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}

	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if (m_TesselationMesh)
	{
		delete m_TesselationMesh;
		m_TesselationMesh = 0;
	}

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

	if (m_TesselationShader)
	{
		delete m_TesselationShader;
		m_TesselationShader = 0;
	}

	for( int i = 0; i < NUM_LIGHTS; i++ )
	{
		if( m_Lights[i] )
		{
			delete m_Lights[i];
			m_Lights[i] = 0;
		}
	}
}


bool Lab9::Frame()
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

bool Lab9::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix( worldMatrix );
	m_Camera->GetViewMatrix( viewMatrix );
	m_Direct3D->GetProjectionMatrix( projectionMatrix );

	m_time += m_Timer->GetTime();
	while( m_time > 360 ) m_time -= 360;

	worldMatrix = XMMatrixTranslation( -50, -2, -50 );

	m_PlaneMesh->SendData( m_Direct3D->GetDeviceContext() );
	m_NormalShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Camera, m_Lights );
	m_NormalShader->Render( m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount() );

	worldMatrix = XMMatrixTranslation( 0,0,0 );

	if (m_Input->isKeyDown('U')) tesselationFactor += m_Timer->GetTime() * 2;
	if (m_Input->isKeyDown('Y')) tesselationFactor -= m_Timer->GetTime() * 2;
	if( tesselationFactor < 1.0f ) tesselationFactor = 1.0f;
	if( tesselationFactor > 31.0f ) tesselationFactor = 31.0f;

	m_TesselationMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TesselationShader->SetShaderParameters( m_Direct3D->GetDeviceContext( ), worldMatrix, viewMatrix, projectionMatrix, tesselationFactor, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) );
	m_TesselationShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}