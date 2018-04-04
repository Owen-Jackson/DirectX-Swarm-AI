#pragma once
#ifndef _SHOAL_
#define _SHOAL_

#include "Swarm.h"

class Shoal : public Swarm
{
public:
	Shoal();
	~Shoal();
	void InitialiseSwarm() override;	//initialise without collision grid
	void InitialiseSwarm(CollisionGrid*) override;	//initialise with collision grid
	void SetIsRotatingClockwise(bool);
private:
	bool m_rotatingClockwise = true;
};

#endif // !_SHOAL_
