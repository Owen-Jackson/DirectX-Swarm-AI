#pragma once
#ifndef _CAMERA_
#define _CAMERA_

#include <DirectXMath.h>
#include "Input.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(XMFLOAT3);
	void SetRotation(XMFLOAT3);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	
	void Tick(Input*, float);
	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	float moveSpeed = 25.0f;
	float zoomSpeed = 10.0f;
	float minZoom = -20.0f;
	float maxZoom = -800.0f;
};

#endif // !_CAMERA_
