#pragma once
#ifndef _AGENT_
#define _AGENT_

#include <DirectXMath.h>
#include <algorithm>
#include <list>

using namespace DirectX;

class Agent
{
public:
	Agent();
	~Agent();
	XMMATRIX Tick(XMFLOAT3, float);
	void Separate(std::list<Agent*>);
	void SetPosition(XMFLOAT3);
	void SetRotation(float);

private:
	void Steer(XMFLOAT3);
	void Arrive(XMFLOAT3);
	float m_rot = 0;
	float m_maxSpeed = 2.0f;
	float m_maxForce = 0.2f;	//higher value = faster turning
	XMFLOAT3 m_pos;
	XMFLOAT3 m_acc = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_moveDir;
	XMFLOAT3 m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

#endif // !_AGENT_
