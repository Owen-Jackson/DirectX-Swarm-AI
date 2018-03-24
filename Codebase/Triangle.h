#pragma once
#include "ModelClass.h"

class Triangle : public ModelClass
{
public:
	Triangle();
	~Triangle();
protected:
	void SetupVertices();
	void SetupIndices();
	void SetupCounts();

};