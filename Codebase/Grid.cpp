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
	bool result;


	return true;
}

void Grid::Shutdown()
{
}

bool Grid::Render(ID3D11DeviceContext *)
{
	return false;
}

int Grid::GetIndexCount()
{
	return 0;
}

bool Grid::InitialiseBuffers(ID3D11Device *)
{
	return false;
}

void Grid::ShutdownBuffers()
{
}

void Grid::RenderBuffers(ID3D11DeviceContext *)
{
}
