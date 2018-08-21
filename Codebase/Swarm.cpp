#include "Swarm.h"
#include "Agent.h"

using namespace DirectX;

Swarm::Swarm()
{
	m_model = new Triangle();
}

Swarm::~Swarm()
{
}

void Swarm::InitialiseSwarm()
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Agent* agent = new Agent(this);
			agent->SetPosition(XMFLOAT3(j * m_agentScale + m_pos.x, i * m_agentScale + m_pos.y, 0.0f));
			agent->SetScale(m_agentScale);
			m_agents.push_back(agent);
		}
	}
}

//inisitalise swarm when the collision grid is present
void Swarm::InitialiseSwarm(CollisionGrid* grid)
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Agent* agent = new Agent(this);
			agent->SetPosition(XMFLOAT3(j * 2.0f * m_agentScale + m_pos.x + (j - m_swarmWidth * m_agentScale), i * 2.0f * m_agentScale + m_pos.y + (i - m_swarmHeight * m_agentScale), 0.0f));
			agent->SetScale(m_agentScale);
			agent->SetMaxSpeed(m_agentSpeed);
			m_agents.push_back(agent);

			//add the new agent to the collision grid
			grid->GetCell(agent->GetPosition())->agentList.push_back(agent);
		}
	}
}

void Swarm::Shutdown()
{
	for (std::vector<Agent*>::iterator agent = m_agents.begin(); agent != m_agents.end(); agent++)
	{
		delete (*agent);
	}
	m_agents.clear();
}

void Swarm::Tick(float dt)
{
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

SwarmType& Swarm::GetSwarmType()
{
	return m_swarmType;
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

void Swarm::SetSwarmSpeed(float agentSpeed)
{
	m_agentSpeed = agentSpeed;
}

void Swarm::SetSwarmType(const SwarmType type)
{
	m_swarmType = type;
}