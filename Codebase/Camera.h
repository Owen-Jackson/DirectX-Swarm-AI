#pragma once
#ifndef _CAMERA_
#define _CAMERA_

#include <DirectXMath.h>
#include "Input.h"

__declspec(align(16)) class Camera
{
public:
	Camera();
	~Camera();

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	void SetPosition(DirectX::XMFLOAT3);
	void SetRotation(DirectX::XMFLOAT3);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	
	void Tick(Input*, float);
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMMATRIX m_viewMatrix;
	float moveSpeed = 50.0f;
	float zoomSpeed = 10.0f;
	float minZoom = -20.0f;
	float maxZoom = -800.0f;
};

#endif // !_CAMERA_
