#include "Graphics.h"

Graphics::Graphics()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	//m_Model = nullptr;
	m_ColorShader = nullptr;
}

Graphics::~Graphics()
{
}

bool Graphics::Initialise(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

	//create the D3D object
	m_D3D = new D3DClass();
	if (!m_D3D)
	{
		return false;
	}

	//initialise the D3D object
	result = m_D3D->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, "Could not initalise Direct3D", "Error", MB_OK);
		return false;
	}

	//create the camera
	m_Camera = new Camera();
	if (!m_Camera)
	{
		return false;
	}

	//set the initial position of the camera
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -50.0f));

	//add swarm(s)	
	Swarm* testSwarm1 = new Swarm();
	testSwarm1->SetInstanceCount(10, 10);
	testSwarm1->SetSwarmColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	testSwarm1->SetSwarmPosition(XMFLOAT3(0, 0, 0));
	m_swarms.push_back(testSwarm1);
	
	/*
	Triangle* testSwarm2 = new Triangle();
	testSwarm2->SetInstanceCount(300, 300);
	testSwarm2->SetColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	testSwarm2->SetPosition(XMFLOAT3(2.0f, 0.0f, 0.0f));
	m_staticModels.push_back(testSwarm2);
	*/

	//add swarm models to the models list
	for (std::list<Swarm*>::iterator swarm = m_swarms.begin(); swarm != m_swarms.end(); swarm++)
	{
		m_models.push_back((*swarm)->GetModel());
	}

	//initialise the static models	
	for (std::list<ModelClass *>::iterator model = m_models.begin(); model != m_models.end(); model++)
	{
		(*model)->Initialise(m_D3D->GetDevice(), hWnd);
		(*model)->RenderBuffers(m_D3D->GetDeviceContext());
	}

	//initialise swarms
	for (std::list<Swarm*>::iterator swarm = m_swarms.begin(); swarm != m_swarms.end(); swarm++)
	{
		(*swarm)->InitialiseSwarm(m_D3D->GetDevice(), hWnd);
	}
	return true;
}

void Graphics::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}

	for (std::list<Swarm *>::iterator swarm = m_swarms.begin(); swarm != m_swarms.end(); swarm++)
	{
		(*swarm)->Shutdown();
		delete (*swarm);
	}
	m_swarms.clear();

	for (std::list<ModelClass *>::iterator model = m_models.begin(); model != m_models.end(); model++)
	{
		(*model)->Shutdown();
		delete (*model);
	}
	m_models.clear();

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
	return;
}


bool Graphics::Tick(Input* input, float dt)
{
	bool result;
	//generate the view matrix based on the camera's position
	m_Camera->Tick(input, dt);

	XMFLOAT3 target;
	target = MouseToWorldCoords(input);
	//tick all of the swarms
	for (std::list<Swarm *>::iterator swarm = m_swarms.begin(); swarm != m_swarms.end(); swarm++)
	{
		(*swarm)->SetTarget(target);
		(*swarm)->Tick(dt);
	}
	
	//tick all of the objects
	for (std::list<ModelClass *>::iterator model = m_models.begin(); model != m_models.end(); model++)
	{
		//(*model)->Tick(dt);
	}
	

	//render the scene
	/*
	result = Render(input);
	if (!result)
	{
		return false;
	}
	*/
	return true;
}


bool Graphics::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//clear buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//setup the matrices
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//transpose the matrices
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//render all of the objects
	for (std::list<ModelClass *>::iterator model = m_models.begin(); model != m_models.end(); model++)
	{
		(*model)->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	}

	//present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}

XMFLOAT3 Graphics::MouseToWorldCoords(Input* input)
{
	float pointX, pointY;
	XMFLOAT2 mousePos = input->GetMouseLocation();
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mousePos.x) / (float)SCREEN_WIDTH) - 1.0f;
	pointY = (((2.0f * (float)mousePos.y) / (float)SCREEN_HEIGHT) - 1.0f) * -1.0f;

	/*
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	worldMatrix = XMMatrixIdentity();
	XMMATRIX transMat = XMMatrixTranslation(pointX, pointY, 0.0f);
	worldMatrix *= transMat;
	
	XMMATRIX worldViewProj = worldMatrix * viewMatrix * projectionMatrix;
	inverseWorldMatrix = XMMatrixInverse(NULL, worldViewProj);

	//XMVECTOR mouseInProj = 
	XMFLOAT3 origin = XMFLOAT3(mousePos.x, -mousePos.y, 0.0f);
	XMVECTOR originVec = XMLoadFloat3(&origin);
	XMVECTOR rayOrigin = XMVector3TransformCoord(originVec, inverseWorldMatrix);

	XMStoreFloat3(&origin, rayOrigin);
	origin.z = 0.0f;
	*/
	
	//adjust points using projection matrix
	XMFLOAT4X4 proj;
	m_D3D->GetProjectionMatrix(projectionMatrix);
	XMStoreFloat4x4(&proj, projectionMatrix);
	pointX = pointX / proj._11;
	pointY = pointY / proj._22;

	//get the inverse of the view matrix
	m_Camera->GetViewMatrix(viewMatrix);
	XMFLOAT4X4 invViewMat;
	inverseViewMatrix = XMMatrixInverse(NULL, viewMatrix);
	XMStoreFloat4x4(&invViewMat, inverseViewMatrix);

	//calculate the direction of the picking ray in view space
	XMFLOAT3 direction;
	direction.x = (pointX * invViewMat._11) + (pointY * invViewMat._21) + invViewMat._31;
	direction.y = (pointX * invViewMat._12) + (pointY * invViewMat._22) + invViewMat._32;
	direction.z = (pointX * invViewMat._13) + (pointY * invViewMat._23) + invViewMat._33;
	XMVECTOR directionVec = XMLoadFloat3(&direction);

	//get the origin by getting the camera position
	XMFLOAT3 origin;
	origin = m_Camera->GetPosition();
	XMVECTOR originVec = XMLoadFloat3(&origin);

	//get the world matrix and translate it
	m_D3D->GetWorldMatrix(worldMatrix);

	//get the inverse of the world matrix
	inverseWorldMatrix = XMMatrixInverse(NULL, worldMatrix);

	//transform the ray origin and ray direction from view space to world space
	XMVECTOR rayOrigin, rayDirection;
	rayOrigin = XMVector3TransformCoord(originVec, inverseWorldMatrix);
	rayDirection = XMVector3TransformNormal(directionVec, inverseWorldMatrix);

	XMStoreFloat3(&origin, rayOrigin);
	origin.z = 0.0f;
	
	return origin;
}