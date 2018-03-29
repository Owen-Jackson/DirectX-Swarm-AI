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

void CollisionGrid::CheckCollisions()
{

}

void CollisionGrid::Tick(float dt)
{
	//CHECK COLLISIONS IN HERE
	for (std::vector<Agent*>::iterator agent = m_allAgents.begin(); agent != m_allAgents.end(); agent++)
	{
		//tick the agent in this cell
		(*agent)->Tick(dt);

		//check if the agent has changed cells
		GridCell* newCell = GetCell((*agent)->GetPosition());
		if (newCell != (*agent)->GetGridCell())
		{
			RemoveAgentFromCell(*agent);
			AddAgentToCell(*agent, newCell);
		}
	}
	

	CheckCollisions();

}