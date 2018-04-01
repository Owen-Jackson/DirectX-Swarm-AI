#include "ModelClass.h"

using namespace DirectX;

ModelClass::ModelClass()
{
	m_vertexBuffer = nullptr;
	m_instanceBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_ColorShader = nullptr;
	m_color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	SetInstanceCount(1, 1);	//default to 1 model
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialise(ID3D11Device* device, HWND hWnd)
{
	bool result;

	//create the color shader
	m_ColorShader = new ColorShaderClass();
	if (!m_ColorShader)
	{
		return false;
	}

	result = m_ColorShader->Initialise(device, hWnd);
	if (!result)
	{
		MessageBox(hWnd, "color shader failed to initialise", "Error", MB_OK);
		return false;
	}

	//initialise the vertex and index buffers
	result = InitialiseBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	//release the vertex and index buffers
	ShutdownBuffers();

	//release color shader
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	result = RemapInstanceBuffer(deviceContext);
	//result = RemapMatrixBuffer(deviceContext);
	if (!result)
	{
		return;
	}

	result = m_ColorShader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return;
	}
	RenderBuffers(deviceContext);
	m_ColorShader->Render(deviceContext, m_vertexCount, m_instanceCount);
	return;
}

int& ModelClass::GetIndexCount()
{
	return m_indexCount;
}

int& ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int& ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}

float& ModelClass::GetScale()
{
	return m_scale;
}

ColorShaderClass* ModelClass::GetColorShader()
{
	return m_ColorShader;
}

XMFLOAT3& ModelClass::GetPosition()
{
	return m_pos;
}

InstanceType* ModelClass::GetInstances()
{
	return m_instances;
}

void ModelClass::SetColor(XMFLOAT4 color)
{
	m_color = color;
}

void ModelClass::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
}

void ModelClass::SetInstanceCount(int width, int height)
{
	m_spawnGridWidth = width;
	m_spawnGridHeight = height;
	m_instanceCount = width * height;
}

void ModelClass::SetScale(float scale)
{
	m_scale = scale;
}

bool ModelClass::InitialiseBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	//create arrays to store the vertex and index data
	SetupCounts();

	//Setup the vertices for the shape here
	SetupVertices();

	//Setup indices for the shape here
	SetupIndices();

	//setup the description for the vertex budder
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//set number of instances
	/*
	InstanceType* instance;
	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			instance = new InstanceType();
			instance->position = XMFLOAT3(-10.0f + (j * 2.0f) + m_pos.x, -10.0f + (i * 2.0f) + m_pos.y, 0.0f + m_pos.z);
			instance->worldMat = XMMatrixIdentity();
			//m_instances.push_back(instance);
		}
	}
	*/

	m_instances = new InstanceType[m_instanceCount]; //(struct InstanceType*)_aligned_malloc(sizeof(struct InstanceType) * m_instanceCount, 16);
	{
		for (int i = 0; i < m_spawnGridHeight; i++)
		{
			for (int j = 0; j < m_spawnGridWidth; j++)
			{
				m_instances[j + i*m_spawnGridHeight].worldMat = XMMatrixScaling(m_scale, m_scale, m_scale) * XMMatrixTranslation(j * m_scale + m_pos.x, i * m_scale + m_pos.y, 0.0f);
				m_instances[j + i*m_spawnGridHeight].worldMat = XMMatrixTranspose(m_instances[j + i*m_spawnGridHeight].worldMat);
			}
		}
	}
	
	if (m_instanceCount > 0)
	{
		//setup the description for the instance buffer
		instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;

		instanceData.pSysMem = m_instances;
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		//create the instance buffer
		result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

void ModelClass::ShutdownBuffers()
{
	//release the instance buffer	
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = nullptr;
	}

	//release the index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	//release the vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	//release the vertex and index arrays
	delete[] m_instances;
	m_instances = nullptr;

	delete[] m_vertices;
	m_vertices = nullptr;

	delete[] m_indices;
	m_indices = nullptr;
	return;
}

bool ModelClass::RemapInstanceBuffer(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mResource;

	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);
	if(FAILED(result))
	{
		return false;
	}

	InstanceType* instanceData = (InstanceType*)mResource.pData;

	for (int i = 0; i < m_instanceCount; i++)
	{
		instanceData[i] = m_instances[i];
	}

	deviceContext->Unmap(m_instanceBuffer, 0);

	return true;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//set vertex buffer stride and offset
	if (m_instanceCount > 0)
	{
		unsigned int strides[2];
		unsigned int offsets[2];
		ID3D11Buffer* bufferPointers[2];

		//set the strides
		strides[0] = sizeof(VertexType);
		strides[1] = sizeof(InstanceType);

		//set the offsets
		offsets[0] = 0;
		offsets[1] = 0;

		//set the buffer pointers
		bufferPointers[0] = m_vertexBuffer;
		bufferPointers[1] = m_instanceBuffer;

		deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	}

	deviceContext->IASetPrimitiveTopology(m_primitiveTopology);

	return;
}

//setup the vertices for the shape in here
void ModelClass::SetupVertices()
{
	return;
}


//setup the indices for the shape in here
void ModelClass::SetupIndices()
{
	return;
}


//setup the vertex and index counts in here
void ModelClass::SetupCounts()
{
	return;
}