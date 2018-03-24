#pragma once
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include "Input.h"
#include "Graphics.h"


class System
{
public:
	System();
	~System();

	bool Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Tick(float);
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();

	LPCSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	Input* m_Input;
	Graphics* m_Graphics;
};

static System* ApplicationHandle = 0;

#endif

