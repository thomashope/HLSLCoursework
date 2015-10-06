// Application.h
#ifndef _LAB5_H
#define _LAB5_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "JellyShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Light.h"

// demonstrates point lights
class Lab5 : public BaseApplication
{
public:

	Lab5(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab5();

	bool Frame();

private:
	bool Render();

private:

	JellyShader* m_JellyShader;
	PointLightShader* m_NormalShader;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	Light* m_Lights[NUM_LIGHTS];
	float m_time;
};

#endif