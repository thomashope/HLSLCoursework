// BaseApplication.h
// Contains base application functionality. For inheritance purposes.
#ifndef _BASEAPPLICATION_H_
#define _BASEAPPLICATION_H_

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 200.0f;	// 1000.0f
const float SCREEN_NEAR = 0.1f;		//0.1f

// Includes
#include "input.h"
#include "d3d.h"
#include "camera.h"
#include "timer.h"

class BaseApplication
{
public:

	BaseApplication(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~BaseApplication();

	virtual bool Frame();

protected:
	virtual void HandleInput(float);
	virtual bool Render() = 0;

protected:
	Input* m_Input;
	D3D* m_Direct3D;
	Camera* m_Camera;
	Timer* m_Timer;
};

#endif