#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	m_position = { 0.0f,0.0f,0.0f };
	m_rotation = { 0.0f,0.0f,0.0f };
	
}

Camera::~Camera()
{
}

void Camera::SetPosition(XMFLOAT3 newPos)
{
	m_position = newPos;
	return;
}

void Camera::SetRotation(XMFLOAT3 newRot)
{
	m_rotation = newRot;
	return;
}

XMFLOAT3 Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3 Camera::GetRotation()
{
	return m_rotation;
}

void Camera::Tick(Input* input, float dt)
{
	if (input->IsKeyPressed(DIK_W))
	{
		m_position.y += moveSpeed * dt;
	}
	if (input->IsKeyPressed(DIK_A))
	{
		m_position.x -= moveSpeed * dt;
	}
	if (input->IsKeyPressed(DIK_S))
	{
		m_position.y -= moveSpeed * dt;
	}
	if (input->IsKeyPressed(DIK_D))
	{
		m_position.x += moveSpeed * dt;
	}

	//scroll to zoom
	float scrollDelta = input->GetMouseWheel();
	if (scrollDelta > 0 && (m_position.z + zoomSpeed) <= minZoom)
	{
		m_position.z += zoomSpeed;
	}
	if (scrollDelta < 0 && (m_position.z - zoomSpeed) >= maxZoom)
	{
		m_position.z -= zoomSpeed;
	}
	Render();
}

void Camera::Render()
{
	//values for up, position and lookat
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	XMFLOAT3 position = m_position;
	XMFLOAT3 lookAt = { 0.0f,0.0f,1.0f };

	//above values stored as vectors for directX math functions
	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	float pitch = XMConvertToRadians(m_rotation.x);
	float yaw = XMConvertToRadians(m_rotation.y);
	float roll = XMConvertToRadians(m_rotation.z);
	XMMATRIX rotMatrix;

	rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotMatrix);
	upVector = XMVector3TransformCoord(upVector, rotMatrix);
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);	

	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}