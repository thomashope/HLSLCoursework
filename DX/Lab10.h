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
#include "TessColourShader.h"
#include "TessNormalShader.h"
#include "BlendShader.h"

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

	// Renders the scene depth information to a texture
	void RenderShadowmap1();
	void RenderShadowmap2();

	// Renders the scene without lighting or shadows
	void RenderScene();

	// Renders the scene with lighting and shadows
	void RenderSceneLighting();

	// Renders the blob in isolation, with any lighting and shadows
	void RenderBlobLighting();

	// Renders the blob normals + alpha for blending + distortion
	void RenderBlobNormals();

	// Blends the various stages together
	void BlendScene();

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
	TessDepthShader* m_TessDepthShader;
	TessColourShader* m_TessColourShader;
	TessNormalShader* m_TessNormalShader;
	BlendShader* m_BlendShader;

	RenderTexture* m_ShadowMap1;
	RenderTexture* m_ShadowMap2;
	RenderTexture* m_BlobDepth;
	RenderTexture* m_SceneLighting;
	RenderTexture* m_BlobLighting;
	RenderTexture* m_BlobNormals;
	RenderTexture* m_BlendedScene;
	
	OrthoMesh* m_FullscreenMesh;
	OrthoMesh* m_TopLeftMesh;
	OrthoMesh* m_TopRightMesh;
	OrthoMesh* m_BottomLeftMesh;
	OrthoMesh* m_BottomRightMesh;

	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	Model* m_Teapot;
	TessellationMesh* m_MagicSphere;

	Light* m_Lights[2];
	float m_time;
	XMFLOAT4 m_frequency;
};

#endif