#pragma once
#include "ModelClass.h"

class Triangle : public ModelClass
{
public:
	Triangle();
	~Triangle();
protected:
	void SetupVertices() override;
	void SetupIndices() override;
	void SetupCounts() override;

};