#pragma once
#ifndef _AGENT_
#define _AGENT_

#include <DirectXMath.h>
#include <vector>

struct GridCell;

class Swarm;

class Agent
{
public:
	Agent(Swarm*);
	virtual ~Agent();
	virtual void Tick(float);
	//void Separate(std::vector<Agent*>&);
	//void Separate(int&);
	DirectX::XMMATRIX SetupWorldMatrix();

	void SetPosition(DirectX::XMFLOAT3);
	void SetRotation(float);
	void SetScale(float);
	void SetGridCell(GridCell*);
	void SetGridCellVectorIndex(int);

	bool IsAlive();

	DirectX::XMFLOAT3& GetPosition();
	DirectX::XMFLOAT3& GetVelocity();
	DirectX::XMFLOAT3& GetAcceleration();
	GridCell* GetGridCell();
	int& GetGridCellVectorIndex();
	float& GetScale();
	//DirectX::XMFLOAT3& GetSeparation();

protected:
	virtual void Steer(DirectX::XMFLOAT3);	//override for different steering behaviours
	void Arrive(DirectX::XMFLOAT3);
	Swarm* m_parentSwarm = nullptr;
	bool m_alive = true;
	float m_rot = 0;
	float m_maxSpeed = 5.0f;
	float m_maxForce = 3.0f;	//higher value = faster turning
	float m_scale = 1.0f;
	float m_drag = 0.2f;
	DirectX::XMFLOAT3* m_target = nullptr;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_acc = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_moveDir;
	DirectX::XMFLOAT3 m_vel = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_sqDistFromTarget = 0;
	float m_minSqDistFromTarget = 10.0f;
	GridCell* m_currentGridCell = nullptr;
	int m_cellVectorIndex = -1;	//where the agent is in it's cell vector
};
#endif // !_AGENT_