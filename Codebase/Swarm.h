#pragma once
#ifndef _SWARMCLASS_
#define _SWARMCLASS_

#include "Triangle.h"

//this class will move a swarm of models
class Swarm
{
public:
	Swarm();
	~Swarm();
	
	void InitialiseSwarm(ID3D11Device*, HWND);
	void Shutdown();
	void Tick(float);

	Triangle* GetModel();

	void SetInstanceCount(int, int);
	void SetTarget(XMFLOAT3);
	void SetSwarmColor(XMFLOAT4);
	void SetSwarmPosition(XMFLOAT3);

protected:
	Triangle* m_model;

	std::list<Agent*> m_agents;
	InstanceType* m_instances;

	//swarm values
	//initial width and height of the swarm
	int m_swarmWidth;
	int m_swarmHeight;

	XMFLOAT3 m_pos;
	XMFLOAT3 m_swarmTarget;
};

#endif // !_SWARMCLASS_
