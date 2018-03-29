#pragma once
#include <vector>

class Agent;

struct GridCell
{
	int cellNumber;
	std::vector<Agent*> agentList;
};