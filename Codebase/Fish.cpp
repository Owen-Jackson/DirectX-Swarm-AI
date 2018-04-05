#include "Fish.h"
#include "Shoal.h"
#include "Helpers.h"

using namespace DirectX;

Fish::Fish(Swarm* swarm, bool rotatingClockwise) : Agent(swarm)
{
	m_maxSpeed = 5.0f;
	m_minSqDistFromTarget = 500.0f;
	m_isRotatingClockwise = rotatingClockwise;
}

Fish::~Fish()
{
}

void Fish::Tick(float dt)
{
	XMFLOAT3 target = m_parentSwarm->GetTarget();

	//move the agent in their move direction
	Steer(target);

	float velMultiplier = Helpers::Clip((m_minSqDistFromTarget/m_sqDistFromTarget), 0.5f, 1.0f);

	m_vel.x = m_acc.x * dt * m_maxSpeed * velMultiplier;
	m_vel.y = m_acc.y * dt * m_maxSpeed * velMultiplier;

	//apply drag
	m_vel.x *= (1 - dt * m_drag);
	m_vel.y *= (1 - dt * m_drag);

	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
}

void Fish::SetIsRotatingClockwise(bool isClockwise)
{
	m_isRotatingClockwise = isClockwise;
}

void Fish::Steer(XMFLOAT3 target)
{
	//get the acceleration toward the target
	XMVECTOR finalAcc = XMVectorSubtract(XMLoadFloat3(&target), XMLoadFloat3(&m_pos));

	//store the distance from the centre of the swarm
	XMFLOAT3 sqDistanceFromCentre;
	XMStoreFloat3(&sqDistanceFromCentre, XMVector3LengthSq(finalAcc));
	m_sqDistFromTarget = sqDistanceFromCentre.x;

	//calculate the vector needed to steer towards the desired acceleration
	finalAcc = XMVector3Normalize(finalAcc);
	finalAcc *= m_maxSpeed;

	XMFLOAT3 desiredAccFloat;
	XMStoreFloat3(&desiredAccFloat, finalAcc);
	
	//calculate how much to rotate the desired vector by
	float rotationAngle = 0;
	if (m_isRotatingClockwise)
	{
		rotationAngle = Helpers::Clip(90.0f - (1 / (m_minSqDistFromTarget / m_sqDistFromTarget)), 0.0f, 90.0f);
	}
	else
	{
		rotationAngle = Helpers::Clip(-90.0f + (1 / (m_minSqDistFromTarget / m_sqDistFromTarget)), -90.0f, 0.0f);
	}

	float rad = XMConvertToRadians(rotationAngle);
	float x = desiredAccFloat.x;
	float y = desiredAccFloat.y;

	//rotate the desired vector
	desiredAccFloat.x = x * cos(rad) - y * sin(rad);
	desiredAccFloat.y = y * cos(rad) + x * sin(rad);
	m_rot = std::atan2f(desiredAccFloat.y, desiredAccFloat.x);
	finalAcc = XMLoadFloat3(&desiredAccFloat);

	//steering vector = current acceleration - desired acceleration
	XMVECTOR steeringVec = XMVectorSubtract(finalAcc, XMLoadFloat3(&m_acc));
	steeringVec = XMVector3ClampLength(steeringVec, 0.0f, m_maxForce);
	XMStoreFloat3(&m_moveDir, steeringVec);

	m_acc.x += m_moveDir.x;
	m_acc.y += m_moveDir.y;
}