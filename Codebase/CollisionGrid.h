#pragma once
#ifndef _COLLISION_GRID_
#define _COLLISION_GRID_

#include "Grid.h"
#include "GridCell.h"
#include "Agent.h"
#include <vector>

//this class will be used for checking swarm collisions
class CollisionGrid : public Grid
{
public:
	CollisionGrid();
	CollisionGrid(int, int, float);
	~CollisionGrid();

	GridCell* GetCell(int, int);	//gets the cell at coordidnates
	GridCell* GetCell(XMFLOAT3);	//get which cell contains this position

	void AddSwarm(std::vector<Agent*>);	//adds all agents in a swarm to the grid
	void AddAgentToCell(Agent*);	//adds an agent to a cell's list of agents
	void AddAgentToCell(Agent*, GridCell*);	//adds an agent to a specified cell's list of agents
	void RemoveAgentFromCell(Agent*);
	void CheckCollisions();

	void Tick(float) override;
private:
	std::vector<GridCell> m_gridCells;
	std::vector<Agent*> m_allAgents;	//all agents in the scene
	int m_numberOfXCells;
	int m_numberOfYCells;
};

#endif // !_COLLISION_GRID_
