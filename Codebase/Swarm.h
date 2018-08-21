#pragma once
#ifndef _SWARMCLASS_
#define _SWARMCLASS_

#include "Triangle.h"
#include "CollisionGrid.h"
#include "SwarmTypes.h"

class Agent;

//this class will move a swarm of models
class Swarm
{
public:
	Swarm();
	virtual ~Swarm();

	virtual void InitialiseSwarm();	//initialise without collision grid
	virtual void InitialiseSwarm(CollisionGrid*);	//initialise with collision grid
	void Shutdown();
	void Tick(float);

	Triangle* GetModel();
	std::vector<Agent*>& GetSwarm();
	DirectX::XMFLOAT3& GetTarget();
	SwarmType& GetSwarmType();

	void SetInstanceCount(int, int);
	void SetTarget(DirectX::XMFLOAT3);
	void SetSwarmColor(DirectX::XMFLOAT4);
	void SetSwarmPosition(DirectX::XMFLOAT3);
	void SetScale(float);
	void SetSwarmSpeed(float);
	void SetSwarmType(const SwarmType);

protected:
	Triangle* m_model;

	std::vector<Agent*> m_agents;
	InstanceType* m_instances;

	//swarm values
	//initial width and height of the swarm
	int m_swarmWidth;
	int m_swarmHeight;
	float m_agentScale = 1.0f;
	float m_agentSpeed = 5.0f;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_swarmTarget;
	SwarmType m_swarmType;
};

#endif // !_SWARMCLASS_
