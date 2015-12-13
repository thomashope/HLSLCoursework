// Application.h
#ifndef _LAB9_H
#define _LAB9_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "TessTintShader.h"
#include "TessellationMesh.h"
#include "JellyShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Light.h"

// demonstrates point lights
class Lab9 : public BaseApplication
{
public:

	Lab9(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab9();

	bool Frame();

private:
	bool Render();

private:

	JellyShader* m_JellyShader;
	PointLightShader* m_NormalShader;
	TessColourShader* m_TesselationShader;

	TessellationMesh* m_TesselationMesh;
	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;

	float tesselationFactor;

	Light* m_Lights[NUM_LIGHTS];
	float m_time;
};

#endif