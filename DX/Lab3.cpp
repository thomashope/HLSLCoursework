// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "lab3.h"
#include <iostream>

Lab3::Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Create Mesh object
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/test_diffuse.png");
	m_LightShader = new PointLightNormalShader(m_Direct3D->GetDevice(), hwnd);
	m_colourShader = new ColourShader(m_Direct3D->GetDevice(), hwnd);
	m_SpecularMap = new Texture(m_Direct3D->GetDevice(), L"../res/test_diffuse_light.png");
	m_NormalMap = new Texture(m_Direct3D->GetDevice(), L"../res/test_normal.png");

	m_LightSphere = new SphereMesh(m_Direct3D->GetDevice(), L"../res/test_diffuse_light.png");
	m_Light = new Light;
	m_Light->SetDiffuseColour( 0.8f, 0.8f, 0.8f, 1.0f );
	m_Light->SetAmbientColour( 0.2f, 0.2f, 0.2f, 1.0f );
	m_Light->SetSpecularColour( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Light->SetSpecularPower( 10.0f );
	m_Light->SetDirection( 1.0f, -1.0f, 0.0f );
}


Lab3::~Lab3()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	if (m_SpecularMap) delete m_SpecularMap;
	if (m_NormalMap) delete m_NormalMap;

	// Release the Direct3D object.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}

	if( m_Light )
	{
		delete m_Light;
		m_Light = 0;
	}
}


bool Lab3::Frame()
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

bool Lab3::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.0f, 0.19f, 0.22f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// rotate the light
	static float angle = 0.0f;
	//m_Light->SetDirection(sinf(angle), -1.0f, 0.0f);
	angle += m_Timer->GetTime();


	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixMultiply(XMMatrixScaling(0.1f, 0.1f, 0.1f), XMMatrixTranslation(-1.0f, -5.0f, -11.0f) );

	// Send geometry data (from mesh)
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		m_PlaneMesh->GetTexture(), m_SpecularMap->GetTexture(), m_NormalMap->GetTexture(), m_Camera, m_Light);
	// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}