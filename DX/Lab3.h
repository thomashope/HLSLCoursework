// Application.h
#ifndef _LAB3_H
#define _LAB3_H

// Includes
#include "baseapplication.h"

#include "PointLightNormalShader.h"
#include "ColourShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Texture.h"
#include "Light.h"

class Lab3 : public BaseApplication
{
public:

	Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab3();

	bool Frame();

private:
	bool Render();

private:

	PointLightNormalShader* m_LightShader;	
	ColourShader* m_colourShader;

	PlaneMesh* m_PlaneMesh;
	Light* m_Light;
	SphereMesh* m_LightSphere;

	Texture* m_SpecularMap;
	Texture* m_NormalMap;
};

#endif