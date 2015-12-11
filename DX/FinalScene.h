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
#include "ShowNormalShader.h"
#include "NormalShadowShader.h"
#include "BoxBlurShader.h"
#include "DOFShader.h"

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

	void BlurScene();

	// takes all the textures and displays them
	void ShowScene();

	void ShowDOF();

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
	NormalShadowShader* m_NormalShadowShader;
	TessDepthShader* m_TessDepthShader;
	TessColourShader* m_TessColourShader;
	TessNormalShader* m_TessNormalShader;
	BlendShader* m_BlendShader;
	ShowNormalShader* m_ShowNormalShader;
	BoxBlurShader* m_BlurShader;
	DOFShader* m_DOFShader;

	RenderTexture* m_ShadowMap1;	// shadow map for the first light
	RenderTexture* m_ShadowMap2;	// shadow map for the second light
	RenderTexture* m_SceneLighting;	// scene with light and shadow, without the magic sphere
	RenderTexture* m_BlobLighting;	// magic sphere diffuse colours
	RenderTexture* m_BlobNormals;	// magic sphere normals
	RenderTexture* m_BlendedScene;	// scene with light and shadow, blended with magic sphere
	RenderTexture* m_BluredScene;	// blurred version of the blended scene
	
	OrthoMesh* m_FullscreenMesh;	// used for multi pass rendering
	OrthoMesh* m_TopLeftMesh;		// top left quadrant
	OrthoMesh* m_TopRightMesh;		// top right quadrant
	OrthoMesh* m_BottomLeftMesh;	// bottom left quadrant
	OrthoMesh* m_BottomRightMesh;	// bottom righ quadrant

	PlaneMesh* m_FloorMesh;				// a plane for the floor
	Model* m_Hellknight;				// hellknight model
	TessellationMesh* m_MagicSphere;	// starts as a cube, tesselates into a sphere

	XMFLOAT4 m_frequency;				// distortion frequency for the tesselated mesh
	Light* m_Lights[2];					// array of shadow casting lights
	float m_time;						// accumulated time in the scene

	bool m_showNormals;					// user controled bool to display model normals
	float m_tesselation;				// user controled tesselation value
};

#endif