#pragma once
#ifndef _INPUT_
#define _INPUT_

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>

class Input
{
public:
	Input();
	~Input();

	bool Initialise(HINSTANCE, HWND, int, int);
	void ShutDown();
	bool Tick(HWND);

	bool IsKeyPressed(const int);
	bool IsEscapePressed();
	DirectX::XMFLOAT2 GetMouseLocation();
	float GetMouseWheel();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput(HWND);

	//direct input members
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	//used to record states of keyboard and mouse
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	float m_mouseWheelDelta;
};

#endif // !_INPUT_
