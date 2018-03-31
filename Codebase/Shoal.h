#pragma once
#ifndef _SHOAL_
#define _SHOAL_

#include "Swarm.h"

class Shoal : public Swarm
{
public:
	Shoal();
	~Shoal();
	void InitialiseSwarm(ID3D11Device*, HWND) override;	//initialise without collision grid
	void InitialiseSwarm(ID3D11Device*, HWND, CollisionGrid*) override;	//initialise with collision grid
	void SetIsRotatingClockwise(bool);
private:
	bool m_rotatingClockwise = true;
};

#endif // !_SHOAL_
