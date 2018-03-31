#pragma once
#ifndef _FISH_
#define _FISH_

#include "Agent.h"

class Fish : public Agent
{
public:
	Fish(Swarm*, bool);
	~Fish();
	virtual void Tick(float) override;
	void SetIsRotatingClockwise(bool);
protected:
	void Steer(DirectX::XMFLOAT3) override;
private:
	bool m_isRotatingClockwise = true;
};

#endif // !_FISH_
