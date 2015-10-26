// Application.h
#ifndef _LAB8_H
#define _LAB8_H

// Includes
#include "baseapplication.h"

#include "PointLightShader.h"
#include "BoxBlurShader.h"
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

	void RenderScene();
	void RenderBlurredScene();
	void RenderDownsampledScene();

	void RenderHorizontalBlur() {}
	void RenderGaussianBlur() {}

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
	BoxBlurShader* m_BoxBlurShader;

	RenderTexture* m_StandardSceneTexture;
	RenderTexture* m_BlurredSceneTexture;
	RenderTexture* m_DownSampledSceneTexture;
	
	RenderTexture* m_HorizontalyBlurredTexture;
	RenderTexture* m_GuassianBlurTexture;
	
	OrthoMesh* m_FullscreenMesh;
	OrthoMesh* m_TopLeftMesh;
	OrthoMesh* m_TopRightMesh;
	OrthoMesh* m_BottomLeftMesh;
	OrthoMesh* m_BottomRightMesh;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;

	Light* m_Lights[NUM_LIGHTS];
	float m_time;
};

#endif