// Application.h
#ifndef _LAB8_H
#define _LAB8_H

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
class Lab8 : public BaseApplication
{
// functons
public:

	Lab8(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab8();

	bool Frame();

private:
	bool Render();

	void RenderToTexture();
	void BlurTexture();
	void DisplayTextures();
	void RenderToBackBuffer();

	template<typename T>
	void deleteIfNotNull(T* resource) {
		if (resource) delete resource, resource = nullptr;
	}

// variables
public:

private:
	JellyShader* m_JellyShader;
	PointLightShader* m_NormalShader;
	TextureShader* m_TextureShader;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;

	RenderTexture* m_StandardSceneTexture;
	OrthoMesh* m_StandardSceneMesh;

	RenderTexture* m_BlurredSceneTexture;
	OrthoMesh* m_BlurredSceneMesh;

	Light* m_Lights[NUM_LIGHTS];
	float m_time;
};

#endif