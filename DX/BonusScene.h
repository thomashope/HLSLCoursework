// Application.h
#ifndef _BONUS_SCENE_H
#define _BONUS_SCENE_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Light.h"

// demonstrates point lights
class BonusScene : public BaseApplication
{
public:

	BonusScene(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~BonusScene();

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