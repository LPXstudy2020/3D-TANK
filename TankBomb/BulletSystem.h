#ifndef BULLET_SYSTEM_H
#define BULLET_SYSTEM_H
#pragma once

#include "Bullet.h"
class BulletSystem
{
public:
	BulletSystem() = default;
	~BulletSystem() = default;
	std::vector<Bullet>* getBulletVes();
	void addBullet(Bullet bullet);
	void update(float dt);
	void draw(ID3D11DeviceContext* deviceContext, BasicEffect& basicEffect);
	////解决冲突
	//void virtual  solveCollision(GameObject* obj, float dt);
	void removeBullet();
	void setVisible();
private:
	std::vector<Bullet> mBulletVec;
};
#endif

