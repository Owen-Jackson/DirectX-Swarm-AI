#include "CollisionGrid.h"
#include <string>

CollisionGrid::CollisionGrid()
{
}

CollisionGrid::~CollisionGrid()
{
}

CollisionGrid::CollisionGrid(int width, int height, float scale)
{
	//setup grid size values
	m_gridWidth = width;
	m_gridHeight = height;
	m_scale = scale;
	m_cellSize = scale;
	m_numberOfXCells = m_gridWidth;
	m_numberOfYCells = m_gridHeight;
	m_maxX = m_gridWidth * m_scale;
	m_maxY = m_gridHeight * m_scale;

	//resize the grid cells grid
	m_gridCells.resize(m_numberOfXCells * m_numberOfYCells);
	for (int i = 0; i < m_gridCells.size() - 1; i++)
	{
		m_gridCells[i].cellNumber = i;
	}
}

GridCell* CollisionGrid::GetCell(int x, int y)
{
	//checks to make sure the x and y inputs are correct
	if (x < 0)
	{
		x = 0;
	}

	if (x >= m_numberOfXCells)
	{
		x = m_numberOfXCells - 1;
	}

	if (y < 0)
	{
		y = 0;
	}

	if (y >= m_numberOfYCells)
	{
		y = m_numberOfYCells - 1;
	}

	return &m_gridCells[y * m_numberOfXCells + x];
}

GridCell* CollisionGrid::GetCell(XMFLOAT3 pos)
{
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return GetCell(cellX, cellY);
}

float& CollisionGrid::GetMaxX()
{
	return m_maxX;
}

float& CollisionGrid::GetMaxY()
{
	return m_maxY;
}

void CollisionGrid::AddSwarm(std::vector<Agent*> swarm)
{
	for (std::vector<Agent*>::iterator agent = swarm.begin(); agent != swarm.end(); agent++)
	{
		m_allAgents.push_back(*agent);
		AddAgentToCell(*agent);
	}
}

void CollisionGrid::AddAgentToCell(Agent* agent)
{
	GridCell* cell = GetCell(agent->GetPosition());
	cell->agentList.push_back(agent);
	agent->SetGridCell(cell);
	agent->SetGridCellVectorIndex(cell->agentList.size() - 1);
}

void CollisionGrid::AddAgentToCell(Agent* agent, GridCell* cell)
{
	cell->agentList.push_back(agent);
	agent->SetGridCell(cell);
	agent->SetGridCellVectorIndex(cell->agentList.size() - 1);
}

void CollisionGrid::RemoveAgentFromCell(Agent* agent)
{
	std::vector<Agent*>& agents = agent->GetGridCell()->agentList;

	//use vector swap to remove the agent
	agents[agent->GetGridCellVectorIndex()] = agents.back();
	agents.pop_back();

	//update the vector index
	if (agent->GetGridCellVectorIndex() < agents.size())
	{
		agents[agent->GetGridCellVectorIndex()]->SetGridCellVectorIndex(agent->GetGridCellVectorIndex());
	}

	//set the index of agent back to -1
	agent->SetGridCellVectorIndex(-1);
	agent->SetGridCell(nullptr);
}

void CollisionGrid::CheckWallCollisions(Agent* agent)
{
	//check collisions against the edge of the grid
	//left wall
	if (agent->GetPosition().x - agent->GetScale() < 0)
	{
		agent->GetPosition().x = agent->GetScale();
		if (agent->GetVelocity().x < 0)
		{
			agent->GetVelocity().x = 0;
			agent->GetAcceleration().x = 0;
		}
	}
	//right wall
	else if (agent->GetPosition().x + agent->GetScale() > m_maxX)
	{
		agent->GetPosition().x = m_maxX - agent->GetScale();
		if (agent->GetVelocity().x > 0)
		{
			agent->GetVelocity().x = 0;
			agent->GetAcceleration().x = 0;
		}
	}
	//bottom wall
	if (agent->GetPosition().y - agent->GetScale() < 0)
	{
		agent->GetPosition().y = agent->GetScale();
		if (agent->GetVelocity().y < 0)
		{
			agent->GetVelocity().y = 0;
			agent->GetAcceleration().y = 0;
		}
	}
	//top wall
	else if (agent->GetPosition().y + agent->GetScale() > m_maxY)
	{
		agent->GetPosition().y = m_maxY - agent->GetScale();
		if (agent->GetVelocity().y > 0)
		{
			agent->GetVelocity().y = 0;
			agent->GetAcceleration().y = 0;
		}
	}
}

void CollisionGrid::CheckCollision(Agent* agent, std::vector<Agent*>& agentsToCheck, int startingIndex)
{
	for (int i = startingIndex; i < agentsToCheck.size(); i++)
	{
		CheckCollision(agent, agentsToCheck[i]);
	}
}

void CollisionGrid::CheckCollision(Agent* a1, Agent* a2)
{
	//get the distance between the agents
	XMFLOAT3 a1Scale = XMFLOAT3(a1->GetScale(), a1->GetScale(), 0);
	XMFLOAT3 a2Scale = XMFLOAT3(a2->GetScale(), a2->GetScale(), 0);
	XMVECTOR a1Radius = XMVectorAdd(XMLoadFloat3(&a1->GetPosition()), XMLoadFloat3(&a1Scale));
	XMVECTOR a2Radius = XMVectorAdd(XMLoadFloat3(&a2->GetPosition()), XMLoadFloat3(&a2Scale));
	XMVECTOR distanceVec = XMVectorSubtract(a2Radius, a1Radius);
	XMFLOAT3 distance;
	XMFLOAT3 distanceDir;
	XMStoreFloat3(&distance, XMVector3Length(distanceVec));
	XMStoreFloat3(&distanceDir, XMVector3Normalize(distanceVec));
	//get the radius of the two agents
	float totalRadius = a1->GetScale() + a2->GetScale();

	float collisionDepth = totalRadius - distance.x;

	//check for collision
	if (collisionDepth > 0)
	{
		//push away the second agent
		a1->GetPosition().x -= distanceDir.x * collisionDepth;
		a1->GetPosition().y -= distanceDir.y * collisionDepth;
		a1->GetPosition().z -= distanceDir.z * collisionDepth;
	}
}

void CollisionGrid::UpdateCollisions()
{
	for (int i = 0; i < m_gridCells.size(); i++)
	{
		if (m_gridCells[i].agentList.size() > 0)
		{
			int x = i % m_numberOfXCells;
			int y = i / m_numberOfXCells;

			GridCell& cell = m_gridCells[i];

			//loop through all agents in the cell
			for (int j = 0; j < cell.agentList.size(); j++)
			{
				int count = 0;	//used to count the number of agents to separate from
				Agent* agent = cell.agentList[j];
				//check for this cell
				CheckCollision(agent, cell.agentList, j + 1);

				//check neighbouring cells
				//left checks
				if (x > 0)
				{
					//middle left
					CheckCollision(agent, GetCell(x - 1, y)->agentList, 0);
					//top left
					if (y > 0)
					{
						CheckCollision(agent, GetCell(x - 1, y - 1)->agentList, 0);
					}
					//bottom left
					if (y < m_numberOfYCells - 1)
					{
						CheckCollision(agent, GetCell(x - 1, y + 1)->agentList, 0);
					}
				}
				//above cell
				if (y > 0)
				{
					CheckCollision(agent, GetCell(x, y - 1)->agentList, 0);
				}
			}
		}
	}
}

void CollisionGrid::Tick(float dt)
{
	//Loop through all agents in the grid and tick their movement
	for (std::vector<Agent*>::iterator agent = m_allAgents.begin(); agent != m_allAgents.end(); agent++)
	{
		//tick the agent in this cell
		(*agent)->Tick(dt);

		CheckWallCollisions(*agent);

		//check if the agent has changed cells
		GridCell* newCell = GetCell((*agent)->GetPosition());
		if (newCell != (*agent)->GetGridCell())
		{
			RemoveAgentFromCell(*agent);
			AddAgentToCell(*agent, newCell);
		}
	}
	
	//check and update collisions for each agent by using the grid cells also separate the agents (boids)
	UpdateCollisions();

	//if there is a collision here then separate the agents
}