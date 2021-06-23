#include "Bullet.h"



void Bullet::setPosition(const DirectX::XMFLOAT3& position) 
{
	this->GetTransform().SetPosition(position);
}

DirectX::XMFLOAT3 Bullet::getPosition()const
{
	return this->GetTransform().GetPosition();
}

void Bullet::setScale(float x, float y,float z) 
{
	this->GetTransform().SetScale(x,y,z);
}

DirectX::XMFLOAT3 Bullet::getScale()const 
{
	return this->GetTransform().GetScale();
}
void Bullet::setSpeed(float speed) 
{
	mSpeed = speed;
}
float Bullet::getSpeed() const
{
	return mSpeed;
}

void Bullet::setDirection(const DirectX::XMFLOAT3& direction)
{
	mDirection = direction;
}

void Bullet::yaw(float radian)
{
	DirectX::XMFLOAT3 rotation = GetTransform().GetRotation();
	rotation.z = DirectX::XMScalarModAngle(rotation.z + radian);
	GetTransform().SetRotation(rotation);
}

bool Bullet::getVisual()const
{
	return isVisual;
}

void Bullet::setVisual(bool d)
{
	isVisual = d;
}

void Bullet::update(float dt) 
{
	mDirection.y += dt * fallSpeed;
	yaw(dt*rotateSpeed);
	GetTransform().Translate(mDirection,-dt*mSpeed);
}


void Bullet::solveCollision(GameObject * obj, float dt)
{
	int rondom = rand() % 4 + 1;
	obj->decreaseHp(rondom*10);
	setVisual(false);
}

bool Bullet::isCollision(GameObject * obj)
{
	if (isVisual == true&&obj != NULL && GetBoundingBox().Intersects(obj->GetBoundingBox()))
	{
		return true;
	}
	else
		return false;
}

float Bullet::getFallSpeed() 
{
	return fallSpeed;
}
float Bullet::getSpeed() 
{
	return mSpeed;
}