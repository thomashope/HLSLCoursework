// Lab3.cpp
// Lab 1 example, simple coloured triangle mesh
#include "lab3.h"

Lab3::Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Create Mesh object
	m_SphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/brick1.dds");
	m_LightShader = new LightShader( m_Direct3D->GetDevice( ), hwnd );
	
	m_Light = new Light;
	m_Light->SetDiffuseColour( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Light->SetDirection( 0.5f, -0.5f, 0.0f );
}


Lab3::~Lab3()
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
	m_Direct3D->BeginScene(0.28f, 0.19f, 0.22f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Send geometry data (from mesh)
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters( m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Light );
	// Render object (combination of mesh geometry and shader process
	m_LightShader->Render( m_Direct3D->GetDeviceContext( ), m_SphereMesh->GetIndexCount( ) );

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}