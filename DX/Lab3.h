// Application.h
#ifndef _LAB3_H
#define _LAB3_H

// Includes
#include "baseapplication.h"

#include "DirectionalLightShader.h"
#include "SphereMesh.h"
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

	DirectionalLightShader* m_LightShader;
	SphereMesh* m_SphereMesh;
	Light* m_Light;
};

#endif