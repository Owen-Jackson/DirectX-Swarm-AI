#pragma once
#ifndef _MODELCLASS_
#define _MODELCLASS_

#include <d3d11.h>
#include <DirectXMath.h>
#include "ColorShaderClass.h"

__declspec(align(16)) struct InstanceType	//passed into the shader
{
	DirectX::XMMATRIX worldMat;
};

class ModelClass
{
protected:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	ModelClass();
	virtual ~ModelClass();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	void Render(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
	virtual void Tick(float) {};

	int& GetIndexCount();
	int& GetVertexCount();
	int& GetInstanceCount();
	float& GetScale();
	DirectX::XMFLOAT3& GetPosition();
	ColorShaderClass* GetColorShader();
	InstanceType* GetInstances();

	void AddInstances(int);
	void RenderBuffers(ID3D11DeviceContext*);
	void SetColor(DirectX::XMFLOAT4);
	void SetPosition(DirectX::XMFLOAT3);
	void SetInstanceCount(int, int);
	void SetScale(float);
	/*
	void SetRotation(XMFLOAT3);
	void SetMoveSpeed(float speed);
	void SetMoveDirection(XMFLOAT3 dir);
	*/
protected:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool RemapInstanceBuffer(ID3D11DeviceContext*);
	
	virtual void SetupVertices();
	virtual void SetupIndices();
	virtual void SetupCounts();

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	ID3D11Buffer* m_indexBuffer;

	VertexType* m_vertices;
	InstanceType* m_instances;
	//InstanceData* m_agentData;
	//std::list<Agent*> m_agents;
	unsigned long* m_indices;

	//width and height of the grid to setup the instances in
	int m_spawnGridWidth;
	int m_spawnGridHeight;

	int m_vertexCount;
	int m_instanceCount;
	int m_indexCount;

	ColorShaderClass* m_ColorShader;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT4 m_color;
	float m_scale = 1;

	//float m_moveSpeed;
	//XMFLOAT3 m_swarmTarget;

	//used when using the mouse
	int m_clientWidth;
	int m_clientHeight;	

	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

#endif // !_MODELCLASS_
