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
			Agent* agent = new Agent();
			agent->SetPosition(XMFLOAT3(-10.0f + j * 2 + m_pos.x, -10.0f + i * 2, 0.0f));
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranslation(-10.0f + j * 2 + m_pos.x, -10.0f + i * 2, 0.0f);
			//m_instances[j + i*m_swarmHeight].worldMat = XMMatrixTranspose(m_instances[j + i*m_swarmHeight].worldMat);
			m_agents.push_back(agent);
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
	for (std::list<Agent*>::iterator agent = m_agents.begin(); agent != m_agents.end(); agent++)
	{
		//(*agent)->Separate(m_agents);
		m_instances[i].worldMat = (*agent)->Tick(m_swarmTarget, dt);
		i++;
		//m_instances[i].worldMat = m_agents[i].Tick(m_swarmTarget, dt);
		/*
		//need to un-transpose to make the matrix editable
		m_instances[i].worldMat = XMMatrixTranspose(m_instances[i].worldMat);

		//get the direction to move to
		movement = XMFLOAT3(m_swarmTarget.x - m_agentData[i].pos.x, m_swarmTarget.y - m_agentData[i].pos.y, 0.0f);
		moveVec = XMVector3Normalize(XMLoadFloat3(&movement));
		XMStoreFloat3(&m_agentData[i].move_dir, moveVec);

		//move the agent in their move direction
		m_agentData[i].pos.x += m_agentData[i].move_dir.x * 0.1f;
		m_agentData[i].pos.y += m_agentData[i].move_dir.y * 0.1f;

		//rotate the agent around its local coords
		//m_agentData[i].rot += 0.1f;
		rotMat = XMMatrixRotationZ(m_agentData[i].rot);
		transMat = XMMatrixTranslation(m_agentData[i].pos.x, m_agentData[i].pos.y, m_agentData[i].pos.z);

		//combine the matrices to get the final world matrix
		m_instances[i].worldMat = rotMat * transMat;
		m_instances[i].worldMat = XMMatrixTranspose(m_instances[i].worldMat);
		*/
	}
}

Triangle* Swarm::GetModel()
{
	return m_model;
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
