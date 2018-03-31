#include "Graphics.h"

using namespace DirectX;

Graphics::Graphics()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
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

	//collision test grid
	CollisionGrid* collisionGrid = new CollisionGrid(100, 100, 5);
	collisionGrid->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
	m_models.push_back(collisionGrid);

	//set the initial position of the camera
	XMFLOAT3 gridCentre;
	gridCentre.x = collisionGrid->GetMaxX() / 2;
	gridCentre.y = collisionGrid->GetMaxY() / 2;
	gridCentre.z = 0;
	m_Camera->SetPosition(XMFLOAT3(gridCentre.x, gridCentre.y, -50.0f));

	//add swarm(s)		
	Shoal* greenShoal = new Shoal();
	greenShoal->SetInstanceCount(50, 50);
	greenShoal->SetSwarmColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	greenShoal->SetSwarmPosition(XMFLOAT3(gridCentre.x + (gridCentre.x / 2), gridCentre.y + (gridCentre.y / 2), 0));
	greenShoal->SetSwarmType(SwarmType::PREY);
	greenShoal->SetTarget(XMFLOAT3(gridCentre.x + (gridCentre.x / 2), gridCentre.y + (gridCentre.y / 2), 0));
	greenShoal->SetIsRotatingClockwise(false);
	m_swarms.push_back(greenShoal);

	//add swarm(s)		
	Shoal* cyanShoal = new Shoal();
	cyanShoal->SetInstanceCount(50, 50);
	cyanShoal->SetSwarmColor(XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	cyanShoal->SetSwarmPosition(XMFLOAT3(gridCentre.x - (gridCentre.x / 2), gridCentre.y - (gridCentre.x / 2), 0));
	cyanShoal->SetSwarmType(SwarmType::PREY);
	cyanShoal->SetTarget(XMFLOAT3(gridCentre.x - (gridCentre.x / 2), gridCentre.y - (gridCentre.x / 2), 0));
	m_swarms.push_back(cyanShoal);

	//add swarm(s)		
	Shoal* purpleShoal = new Shoal();
	purpleShoal->SetInstanceCount(33, 33);
	purpleShoal->SetSwarmColor(XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f));
	purpleShoal->SetSwarmPosition(XMFLOAT3(gridCentre.x - (gridCentre.x / 2), gridCentre.y + (gridCentre.x / 2), 0));
	purpleShoal->SetSwarmType(SwarmType::PREY);
	purpleShoal->SetTarget(XMFLOAT3(gridCentre.x - (gridCentre.x / 2), gridCentre.y + (gridCentre.x / 2), 0));
	purpleShoal->SetIsRotatingClockwise(false);
	m_swarms.push_back(purpleShoal);

	//add swarm(s)		
	Shoal* orangeShoal = new Shoal();
	orangeShoal->SetInstanceCount(100, 100);
	orangeShoal->SetSwarmColor(XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f));
	orangeShoal->SetSwarmPosition(XMFLOAT3(gridCentre.x + (gridCentre.x / 2), gridCentre.y - (gridCentre.x / 2), 0));
	orangeShoal->SetSwarmType(SwarmType::PREY);
	orangeShoal->SetTarget(XMFLOAT3(gridCentre.x + (gridCentre.x / 2), gridCentre.y - (gridCentre.x / 2), 0));
	m_swarms.push_back(orangeShoal);
	
	Swarm* sharks = new Swarm();
	sharks->SetInstanceCount(3, 3);
	sharks->SetSwarmColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
	sharks->SetSwarmPosition(gridCentre);
	sharks->SetSwarmType(SwarmType::PREDATOR);
	sharks->SetScale(2.0f);
	m_swarms.push_back(sharks);
	

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
		(*swarm)->InitialiseSwarm(m_D3D->GetDevice(), hWnd, collisionGrid);

		//add the initialsed swarm's agents to the collision grid
		collisionGrid->AddSwarm((*swarm)->GetSwarm());
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
	//generate the view matrix based on the camera's position
	m_Camera->Tick(input, dt);

	XMFLOAT3 target;
	target = MouseToWorldCoords(input);

	//tick all of the swarms
	for (std::list<Swarm *>::iterator swarm = m_swarms.begin(); swarm != m_swarms.end(); swarm++)
	{
		if ((*swarm)->GetSwarmType() == SwarmType::PREDATOR)
		{
			(*swarm)->SetTarget(target);
		}
		(*swarm)->Tick(dt);
	}
	
	//tick all of the objects
	for (std::list<ModelClass *>::iterator model = m_models.begin(); model != m_models.end(); model++)
	{
		(*model)->Tick(dt);
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
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, inverseWorldMatrix;

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