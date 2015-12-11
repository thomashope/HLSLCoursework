// System.h
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

// INCLUDES 
#include <windows.h>
#include "Lab1.h"
#include "Lab2.h"
#include "Lab3.h"
#include "Lab4.h"
#include "Lab5.h"
#include "Lab6.h"
#include "Lab8.h"
#include "Lab9.h"
#include "FinalScene.h"
#include "Lab11.h"
#include "Input.h"

#define BONUS_SCENE Lab4
#define FINAL_SCENE Lab10

class System
{
public:
	System();
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	
	FINAL_SCENE* m_Application;
	BONUS_SCENE* m_BonusApp;
	Input m_Input;

	bool m_bonus;

//Prototypes
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

//Globals
static System* ApplicationHandle = 0;

#endif