#include "System.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

System::System()
{
	m_Input = nullptr;
	m_Graphics = nullptr;
}

System::~System()
{
}

//initialises all of the stuff for creating a window and handling user input
bool System::Initialise()
{
	int screenWidth = 0, screenHeight = 0;
	bool result;

	InitialiseWindows(screenWidth, screenHeight);

	//create input object for handling user input
	m_Input = new Input();
	if (!m_Input)
	{
		return false;
	}

	//initialise the input
	result = m_Input->Initialise(m_hInstance, m_hWnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hWnd, "Could not initialise the input object", "Error", MB_OK);
		return false;
	}

	//create graphics object for handling the rendering
	m_Graphics = new Graphics();
	if (!m_Graphics)
	{
		return false;
	}

	//initialise the graphics
	result = m_Graphics->Initialise(screenWidth, screenHeight, m_hWnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void System::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_Input)
	{
		m_Input->ShutDown();
		delete m_Input;
		m_Input = nullptr;
	}

	//shutdown the window
	ShutdownWindows();
	return;
}

void System::Run()
{
	MSG msg;
	bool running, result;

	float dt = 0;

	ZeroMemory(&msg, sizeof(msg));

	//loop until a quit message is sent
	running = true;
	while (running)
	{
		//handle windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//end the loop if told to quit
		if (msg.message == WM_QUIT)
		{
			running = false;
		}
		else
		{
			//run the application
			auto start = std::chrono::steady_clock::now();
			result = m_Input->Tick(m_hWnd);
			if (!result)
			{
				running = false;
			}


			result = Tick(dt);
			if (!result)
			{
				running = false;
			}

			//render the scene
			result = m_Graphics->Render();
			if (!result)
			{
				running = false;
			}
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<float> runtime = end - start;
			dt = runtime.count();
		}
		//check if escape has been pressed to exit the application
		if (m_Input->IsEscapePressed())
		{
			running = false;
		}
	}
}

bool System::Tick(float dt)
{
	bool result;
	//tick the graphics objects
	result = m_Graphics->Tick(m_Input, dt);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

//create the window for the application
void System::InitialiseWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//get an external pointer to this object
	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	ZeroMemory(&wc, sizeof(wc));

	//set the name of the application
	m_applicationName = "Swarm AI";

	//setup window class settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)WHITE_BRUSH;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//register the window class
	RegisterClassEx(&wc);

	//get the resolution of the monitor screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//setup the screen settings depending on whether it is fullscreen or windowed
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = 0;
		posY = 0;

		//create a window with all of the settings
		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_applicationName,
			m_applicationName,
			WS_OVERLAPPEDWINDOW,
			posX,
			posY,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			m_hInstance,
			NULL);
	}
	else
	{
		screenWidth = SCREEN_WIDTH;
		screenHeight = SCREEN_HEIGHT;

		RECT rc = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		//create a window with all of the settings
		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_applicationName,
			m_applicationName,
			WS_OVERLAPPEDWINDOW,
			posX,
			posY,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			m_hInstance,
			NULL);
	}
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return;
}

void System::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//fix display settings if exiting fullscreen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//remove the window
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	//remove the application instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	//release the pointer to this class
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
	}
	}
}