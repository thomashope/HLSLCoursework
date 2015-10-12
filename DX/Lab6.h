// Application.h
#ifndef _LAB6_H
#define _LAB6_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "JellyShader.h"
#include "TextureShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "OrthoMesh.h"
#include "RenderTexture.h"
#include "Light.h"

// demonstrates point lights
class Lab6 : public BaseApplication
{
public:

	Lab6(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab6();

	bool Frame();

private:
	bool Render();

private:

	JellyShader* m_JellyShader;
	PointLightShader* m_NormalShader;
	TextureShader* m_TextureShader;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	OrthoMesh* m_OrthoMesh;
	RenderTexture* m_RenderTexture;

	Light* m_Lights[NUM_LIGHTS];
	float m_time;
};

#endif