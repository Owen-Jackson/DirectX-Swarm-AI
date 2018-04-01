#pragma once
#ifndef _COLLISION_GRID_
#define _COLLISION_GRID_

#include "Grid.h"
#include "GridCell.h"
#include <vector>

class Agent;

//this class will be used for checking swarm collisions
class CollisionGrid : public Grid
{
public:
	CollisionGrid();
	CollisionGrid(int width, int height, float scale);
	~CollisionGrid();

	GridCell* GetCell(int, int);	//gets the cell at coordidnates
	GridCell* GetCell(DirectX::XMFLOAT3);	//get which cell contains this position
	float& GetMaxX();
	float& GetMaxY();

	void AddSwarm(std::vector<Agent*>);	//adds all agents in a swarm to the grid
	void AddAgentToCell(Agent*);	//adds an agent to a cell's list of agents
	void AddAgentToCell(Agent*, GridCell*);	//adds an agent to a specified cell's list of agents
	void RemoveAgentFromCell(Agent*);
	void CheckWallCollisions(Agent*);
	void CheckCollision(Agent*, std::vector<Agent*>&, int);		//collision check between an agent and a group of other agents
	void CheckCollision(Agent*, Agent*);						//returns true if the two agents are colliding
	void UpdateCollisions();
	//int CheckSeparation(Agent*, std::vector<Agent*>&, int);
	//bool CheckSeparation(Agent*, Agent*);
	//void UpdateSeparations();

	void Tick(float) override;
private:
	std::vector<GridCell> m_gridCells;
	std::vector<Agent*> m_allAgents;	//all agents in the scene
	int m_numberOfXCells;
	int m_numberOfYCells;
	float m_maxX = 0;	//max x bounds
	float m_maxY = 0;	//max y bounds
};

#endif // !_COLLISION_GRID_
