#include "Swarm.h"

Swarm::Swarm()
{
	m_model = new Triangle();
}

Swarm::~Swarm()
{
}

void Swarm::InitialiseSwarm(ID3D11Device* device, HWND hWnd)
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Agent* agent = new Agent(this);
			agent->SetPosition(XMFLOAT3(j * 2 + m_pos.x, i * 2, 0.0f));
			agent->SetScale(m_agentScale);
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranslation(-10.0f + j * 2 + m_pos.x, -10.0f + i * 2, 0.0f);
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranspose(m_instances[j + i*m_swarmHeight].worldMat);
			m_agents.push_back(agent);
		}
	}
}

void Swarm::InitialiseSwarm(ID3D11Device* device, HWND hWnd, CollisionGrid* grid)
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Agent* agent = new Agent(this);
			agent->SetPosition(XMFLOAT3(j * 2 + m_pos.x, i * 2, 0.0f));
			agent->SetScale(m_agentScale);
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranslation(-10.0f + j * 2 + m_pos.x, -10.0f + i * 2, 0.0f);
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranspose(m_instances[j + i*m_swarmHeight].worldMat);
			m_agents.push_back(agent);

			//add the new agent to the collision grid
			grid->GetCell(agent->GetPosition())->agentList.push_back(agent);
		}
	}
}

void Swarm::Shutdown()
{
	m_model->Shutdown();
	delete m_model;
}

void Swarm::Tick(float dt)
{
	XMMATRIX scaleMat, rotMat, transMat;
	XMVECTOR moveVec;
	XMFLOAT3 movement;
	//for (int i = 0; i < m_instanceCount; i++)
	int i = 0;
	for (std::vector<Agent*>::iterator agent = m_agents.begin(); agent != m_agents.end(); agent++)
	{
		m_instances[i].worldMat = (*agent)->SetupWorldMatrix();
		i++;
	}
}

Triangle* Swarm::GetModel()
{
	return m_model;
}

std::vector<Agent*>& Swarm::GetSwarm()
{
	return m_agents;
}

XMFLOAT3& Swarm::GetTarget()
{
	return m_swarmTarget;
}

void Swarm::SetInstanceCount(int swarmWidth, int swarmHeight)
{
	m_swarmWidth = swarmWidth;
	m_swarmHeight = swarmHeight;
	m_model->SetInstanceCount(swarmWidth, swarmHeight);
}

void Swarm::SetTarget(XMFLOAT3 target)
{
	m_swarmTarget = target;
}

void Swarm::SetSwarmColor(XMFLOAT4 color)
{
	m_model->SetColor(color);
}

void Swarm::SetSwarmPosition(XMFLOAT3 position)
{
	m_pos = position;
	m_model->SetPosition(position);
}

void Swarm::SetScale(float scale)
{
	m_agentScale = scale;
}