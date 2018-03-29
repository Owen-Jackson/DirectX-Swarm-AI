#pragma once
#ifndef _AGENT_
#define _AGENT_

#include <DirectXMath.h>
#include <algorithm>
#include <vector>
#include "CollisionGrid.h"

using namespace DirectX;

class Swarm;

class Agent
{
public:
	Agent(Swarm*);
	~Agent();
	void Tick(float);
	XMMATRIX SetupWorldMatrix();
	void Separate(std::vector<Agent*>);
	void SetPosition(XMFLOAT3);
	void SetRotation(float);
	void SetScale(float);
	void SetGridCell(GridCell*);
	void SetGridCellVectorIndex(int);

	bool IsAlive();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetVelocity();
	GridCell* GetGridCell();
	int GetGridCellVectorIndex();

private:
	virtual void Steer(XMFLOAT3);	//override for different steering behaviours
	void Arrive(XMFLOAT3);
	bool m_alive = true;
	float m_rot = 0;
	float m_maxSpeed = 2.0f;
	float m_maxForce = 0.2f;	//higher value = faster turning
	float m_scale = 1.0f;
	XMFLOAT3* m_target = nullptr;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_acc = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_moveDir;
	XMFLOAT3 m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);	
	GridCell* m_currentGridCell = nullptr;
	int m_cellVectorIndex = -1;	//where the agent is in it's cell vector
	Swarm* m_parentSwarm = nullptr;
};

#endif // !_AGENT_
