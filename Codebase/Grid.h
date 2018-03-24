#pragma once
#ifndef _GRID_
#define _GRID_

#include <d3d11.h>
#include <DirectXMath.h>
#include "ColorShaderClass.h"

using namespace DirectX;

class Grid
{
protected:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Grid();
	~Grid();

	bool Initialise(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	int m_vertexCount, m_indexCount;
};

#endif // !_GRID_
