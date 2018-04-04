#include "Input.h"

using namespace DirectX;

Input::Input()
{
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;
}

Input::~Input()
{
}

bool Input::Initialise(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	//store the screen size, used for positioning mouse cursor
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//initialise mouse location in the screen
	m_mouseX = 0;
	m_mouseY = 0;

	//initialise main direct input interface
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//setup the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//setup the keyboard cooperation level to prevent other applications
	//from being affected by keyboard input while using this application's window
	result = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//acquire the keyboard now that it is setup
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//setup the mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//set mouse cooperation level
	result = m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//acquire the mouse now that it is setup
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::ShutDown()
{
	//release the mouse
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	//release the keyboard
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	//release the interface to direct input
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

bool Input::Tick(HWND hWnd)
{
	bool result;

	//get the keyboard state
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//get the mouse state
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//process the changes in states
	ProcessInput(hWnd);

	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (PVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//if the keyboard lost focus try to reacquire it
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//if the mouse lost focus try to reacquire it
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput(HWND hWnd)
{
	//update the location of the mouse cursor	
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);
	m_mouseX = mousePos.x;
	m_mouseY = mousePos.y;	

	//update mousewheel scroll
	m_mouseWheelDelta = (float)m_mouseState.lZ;

	//make sure the mouse location isn't out of bounds
	if (m_mouseX < 0)
	{
		m_mouseX = 0;
	}
	if (m_mouseY < 0)
	{
		m_mouseY = 0;
	}

	if (m_mouseX > m_screenWidth)
	{
		m_mouseX = m_screenWidth;
	}

	if (m_mouseY > m_screenHeight)
	{
		m_mouseY = m_screenHeight;
	}

	return;
}

//used to check if a key is currently being pressed
bool Input::IsKeyPressed(const int key)
{
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}

//used for quiting the application
bool Input::IsEscapePressed()
{
	//check if escape has been pressed
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

DirectX::XMFLOAT2 Input::GetMouseLocation()
{
	return DirectX::XMFLOAT2((float)m_mouseX, (float)m_mouseY);
}

float Input::GetMouseWheel()
{
	return m_mouseWheelDelta;
}