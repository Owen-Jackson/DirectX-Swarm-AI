#pragma once
#ifndef _GRAPHICS_
#define _GRAPHICS_

#include <Windows.h>
#include <list>
#include "D3DClass.h"
#include "Camera.h"
#include "ShapeIncludes.h"
#include "ColorShaderClass.h"
#include "Swarm.h"

//globals
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Tick(Input*, float);
	bool Render();
	XMFLOAT3 MouseToWorldCoords(Input*);	//converts screen coords to world coords
private:
	D3DClass* m_D3D;
	Camera* m_Camera;
	std::list<ModelClass*> m_models;
	std::list<Swarm*> m_swarms;
	ColorShaderClass* m_ColorShader;
};

#endif