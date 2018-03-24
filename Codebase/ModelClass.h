#pragma once
#ifndef _MODELCLASS_
#define _MODELCLASS_

#include <d3d11.h>
#include <DirectXMath.h>
#include <list>
#include "ColorShaderClass.h"
#include "Agent.h"

using namespace DirectX;

struct InstanceType	//passed into the shader
{
	XMMATRIX worldMat;
};

class ModelClass
{
protected:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	/*
	struct InstanceData	//used for setting up the instance's world matrix each tick
	{
		float rot = 0;
		float speed = 0.1f;
		XMFLOAT3 pos;
		XMFLOAT3 vel;
		XMFLOAT3 move_dir;
		XMFLOAT3 acc;
	};
	*/
public:
	ModelClass();
	~ModelClass();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Tick(float) {};

	int GetIndexCount();
	int GetVertexCount();
	int GetInstanceCount();
	XMFLOAT3 GetPosition();
	InstanceType* GetInstances();

	ColorShaderClass* GetColorShader();

	void AddInstances(int);
	void RenderBuffers(ID3D11DeviceContext*);
	void SetColor(XMFLOAT4);	
	void SetPosition(XMFLOAT3);
	void SetInstanceCount(int, int);
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

	XMFLOAT3 m_pos;
	XMFLOAT4 m_color;
	float m_scale = 1;

	//float m_moveSpeed;
	//XMFLOAT3 m_swarmTarget;

	//used when using the mouse
	int m_clientWidth;
	int m_clientHeight;	

	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

#endif // !_MODELCLASS_
