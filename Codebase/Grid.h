#pragma once
#ifndef _GRID_
#define _GRID_

#include "ModelClass.h"

class Grid : public ModelClass
{
public:
	Grid();
	Grid(int, int);
	~Grid();

	/*
	bool Initialise(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);
	*/

	void SetGridSize(int, int);

protected:
	void SetupVertices() override;
	void SetupIndices() override;
	void SetupCounts() override;

	int m_gridWidth;
	int m_gridHeight;
	float m_cellSize = 1;
};

#endif // !_GRID_
