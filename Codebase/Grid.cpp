#include "Grid.h"

Grid::Grid()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	SetInstanceCount(1, 1);	//the grid only needs one instance
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	m_scale = 5;
}

Grid::Grid(int width, int height)
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetInstanceCount(1, 1);	//the grid only needs one instance
	SetGridSize(width, height);
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	m_scale = 5;
}

Grid::~Grid()
{
}

void Grid::SetGridSize(int width, int height)
{
	m_gridWidth = width;
	m_gridHeight = height;
}

/*
void Grid::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//set the buffers to active so they can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set the render format
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
*/

void Grid::SetupVertices()
{
	//create temporary vertex array
	m_vertices = new VertexType[m_vertexCount];

	int index = 0;
	float posX, posY;
	for (int i = 0; i < (m_gridHeight - 1); i++)
	{
		for (int j = 0; j < (m_gridWidth - 1); j++)
		{
			//top left
			posX = (float)(j);
			posY = (float)(i + 1);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//top right
			posX = (float)(j + 1);
			posY = (float)(i + 1);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//top right
			posX = (float)(j + 1);
			posY = (float)(i + 1);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//bottom right
			posX = (float)(j + 1);
			posY = (float)(i);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//bottom right
			posX = (float)(j + 1);
			posY = (float)(i);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//bottom left
			posX = (float)(j);
			posY = (float)(i);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//bottom left
			posX = (float)(j);
			posY = (float)(i);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;

			//top left
			posX = (float)(j);
			posY = (float)(i + 1);
			m_vertices[index].position = XMFLOAT3(posX, posY, 0.0f);
			m_vertices[index].color = m_color;
			index++;
		}
	}
}

void Grid::SetupIndices()
{
	//create temp index array
	m_indexCount = m_vertexCount;
	m_indices = new unsigned long[m_indexCount];
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_indices[i] = i;
	}
}

void Grid::SetupCounts()
{
	//calculate the number of vertices in the grid
	m_vertexCount = (m_gridWidth - 1) * (m_gridHeight - 1) * 8;
	m_indexCount = m_vertexCount;
}