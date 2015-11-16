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
#include "Lab10.h"
#include "Lab11.h"
#include "Input.h"

#define CURRENT_LAB Lab3

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
	
	CURRENT_LAB* m_Application;
	Input m_Input;

//Prototypes
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

//Globals
static System* ApplicationHandle = 0;

#endif