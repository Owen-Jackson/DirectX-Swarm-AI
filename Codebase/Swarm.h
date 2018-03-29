#pragma once
#ifndef _SWARMCLASS_
#define _SWARMCLASS_

#include "Triangle.h"
#include "CollisionGrid.h"

//this class will move a swarm of models
class Swarm
{
public:
	Swarm();
	~Swarm();
	
	void InitialiseSwarm(ID3D11Device*, HWND);	//initialise without collision grid
	void InitialiseSwarm(ID3D11Device*, HWND, CollisionGrid*);	//initialise with collision grid
	void Shutdown();
	void Tick(float);

	Triangle* GetModel();
	std::vector<Agent*> GetSwarm();
	XMFLOAT3 GetTarget();

	void SetInstanceCount(int, int);
	void SetTarget(XMFLOAT3);
	void SetSwarmColor(XMFLOAT4);
	void SetSwarmPosition(XMFLOAT3);
	void SetScale(float);

protected:
	Triangle* m_model;

	std::vector<Agent*> m_agents;
	InstanceType* m_instances;

	//swarm values
	//initial width and height of the swarm
	int m_swarmWidth;
	int m_swarmHeight;
	float m_agentScale = 1.0f;

	XMFLOAT3 m_pos;
	XMFLOAT3 m_swarmTarget;
};

#endif // !_SWARMCLASS_
