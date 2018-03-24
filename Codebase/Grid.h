#pragma once
#ifndef _GRID_
#define _GRID_

#include <d3d11.h>
#include <DirectXMath.h>
#include "ModelClass.h"

using namespace DirectX;

class Grid : public ModelClass
{
protected:
	/*
	struct NodeType
	{
		float posX, posY, posZ;
		int agentCount;
		ID3D11Buffer* vertexBuffer, *indexBuffer;
		NodeType* childNodes[4];
	};
	*/

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

private:
	int m_gridWidth;
	int m_gridHeight;
};

#endif // !_GRID_
