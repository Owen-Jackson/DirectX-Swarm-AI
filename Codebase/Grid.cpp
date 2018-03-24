#include "Grid.h"

Grid::Grid()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
}

Grid::~Grid()
{
}

bool Grid::Initialise(ID3D11Device* device)
{
	return true;
}