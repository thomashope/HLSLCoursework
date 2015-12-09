// Application.h
#ifndef _LAB10_H
#define _LAB10_H

// Includes
#include "baseapplication.h"
#include "RenderTexture.h"
#include "Light.h"

#include "TextureShader.h"
#include "PointLightShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "TessDepthShader.h"

#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "OrthoMesh.h"
#include "Model.h"
#include "TessellationMesh.h"

// demonstrates point lights
class Lab10 : public BaseApplication
{
// functons
public:

	Lab10(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab10();

	bool Frame();

private:
	bool Render();

	// renders the complete scene
	void RenderSceneDepthOnly();

	void RenderScene();

	void RenderSceneWithShadows();

	// takes all the textures and displays them
	void ShowScene();

	template<typename T>
	void deleteIfNotNull(T* resource) {
		if (resource) delete resource, resource = nullptr;
	}

// variables
public:

private:
	DepthShader* m_DepthShader;
	TextureShader* m_TextureShader;
	ShadowShader* m_ShadowShader;
	TessDepthShader* m_TesselationShader;

	RenderTexture* m_SceneDepth;
	RenderTexture* m_Scene;
	RenderTexture* m_SceneWithShadows;
	
	OrthoMesh* m_FullscreenMesh;
	OrthoMesh* m_TopLeftMesh;
	OrthoMesh* m_TopRightMesh;
	OrthoMesh* m_BottomLeftMesh;
	OrthoMesh* m_BottomRightMesh;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	Model* m_Teapot;
	TessellationMesh* m_MagicSphere;

	Light* m_Lights[1];
	float m_time;
};

#endif