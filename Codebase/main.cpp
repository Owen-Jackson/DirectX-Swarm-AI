#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow)
{
	System* system = nullptr;
	bool result;

	//create the system object
	system = new System();
	if (!system)
	{
		return 0;
	}

	//initialise and run the system object
	result = system->Initialise();
	if (result)
	{
		system->Run();
	}

	//shutdown the system
	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}