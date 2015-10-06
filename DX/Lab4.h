// Application.h
#ifndef _LAB4_H
#define _LAB4_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Light.h"

// demonstrates point lights
class Lab4 : public BaseApplication
{
public:

	Lab4(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab4();

	bool Frame();

private:
	bool Render();

private:

	PointLightShader* m_LightShader;
	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	Light* m_Lights[NUM_LIGHTS];
};

#endif