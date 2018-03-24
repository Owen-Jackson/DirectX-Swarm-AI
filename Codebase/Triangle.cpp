#include "Triangle.h"

Triangle::Triangle() : ModelClass()
{
}

Triangle::~Triangle()
{
}

//Creates an equalateral triangle
void Triangle::SetupVertices()
{
	m_vertices = new VertexType[m_vertexCount];

	//bottom left
	m_vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	m_vertices[0].color = XMFLOAT4(m_color);

	//top middle
	m_vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vertices[1].color = XMFLOAT4(m_color);

	//bottom right
	m_vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_vertices[2].color = XMFLOAT4(m_color);
	return;
}


void Triangle::SetupIndices()
{
	m_indices = new unsigned long[m_indexCount];

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;
	return;
}

void Triangle::SetupCounts()
{
	m_vertexCount = 3;
	m_indexCount = 3;
	return;
}