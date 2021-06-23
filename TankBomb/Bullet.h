#ifndef BULLET_H
#define BULLET_H
#pragma once

#include "GameObject.h"

class Bullet :public GameObject
{
public:
	Bullet() = default;
	~Bullet() = default;

	void setSpeed(float speed);
	float getSpeed() const;
	
	void setDirection(const DirectX::XMFLOAT3& direction);
	
	void setScale(float x, float y, float z);
	DirectX::XMFLOAT3 getScale()const;

	void setPosition(const DirectX::XMFLOAT3& position);
	DirectX::XMFLOAT3 getPosition()const;

	void yaw(float radian);
	bool getVisual() const;
	void setVisual(bool d);
	void update(float dt);

	//碰撞后爆炸，并更新子弹消失
	virtual void  solveCollision(GameObject * obj, float dt) override;
	//碰撞检测
	bool virtual isCollision(GameObject* obj);
	float getFallSpeed();
	float getSpeed();
	
private:
	bool isVisual=true;
	float mSpeed = 50.0f;
	float rotateSpeed = 20.0f;
	float fallSpeed = 0.1f;
	DirectX::XMFLOAT3 mDirection;
};
#endif
