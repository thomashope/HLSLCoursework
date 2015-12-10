// Application.h
#ifndef _LAB11_H
#define _LAB11_H

// Includes
#include "baseapplication.h"
#include "RenderTexture.h"
#include "Light.h"

#include "TextureShader.h"
#include "PointLightNormalShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "GeometryShader.h"

#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "OrthoMesh.h"
#include "PointMesh.h"
#include "Model.h"

// demonstrates point lights
class Lab11 : public BaseApplication
{
// functons
public:

	Lab11(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab11();

	bool Frame();

private:
	bool Render();

	// renders the complete scene
	void RenderSceneDepthOnly();

	void RenderScene();

	void RenderSceneWithShadows();

	void RenderGeometryShader();

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
	GeometryShader* m_GeometryShader;
	PointLightNormalShader* m_MapShader;

	RenderTexture* m_SceneDepth;
	RenderTexture* m_Scene;
	RenderTexture* m_SceneWithShadows;
	RenderTexture* m_GeometryTest;
	
	OrthoMesh* m_FullscreenMesh;
	OrthoMesh* m_TopLeftMesh;
	OrthoMesh* m_TopRightMesh;
	OrthoMesh* m_BottomLeftMesh;
	OrthoMesh* m_BottomRightMesh;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	PointMesh* m_PointMesh;
	Model* m_Teapot;

	Light* m_Lights[1];
	float m_time;
};

#endif