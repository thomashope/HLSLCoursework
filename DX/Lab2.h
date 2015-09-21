// Application.h
#ifndef _LAB2_H
#define _LAB2_H

// Includes
#include "baseapplication.h"

#include "ColourShader.h"
#include "trianglemesh.h"
#include "QuadMesh.h"
#include "TextureShader.h"

class Lab2 : public BaseApplication
{
public:

	Lab2( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* );
	~Lab2();

	bool Frame();

private:
	bool Render();

private:

	ColourShader* m_ColourShader;
	TextureShader* m_TextureShader;
	TriangleMesh* m_TriangleMesh;
	QuadMesh* m_QuadMesh;

};

#endif