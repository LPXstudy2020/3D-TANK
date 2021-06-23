#include "BulletSystem.h"

std::vector<Bullet>* BulletSystem::getBulletVes()
{
	return &mBulletVec;
}

void BulletSystem::addBullet(Bullet bullet)
{
	mBulletVec.push_back(std::move(bullet));
}
void BulletSystem::update(float dt) 
{
	static float time=0;
	time += dt;
	if (time >= 10.0f)
	{
		removeBullet();
		time = 0;
	}
	for (auto& eachBullet : mBulletVec) 
	{
		eachBullet.update(dt);
	}
}

void BulletSystem::draw(ID3D11DeviceContext* deviceContext, BasicEffect& basicEffect) 
{
	for (auto& eachBullet : mBulletVec)
	{

		//可视化为true时，画子弹
		if(eachBullet.getVisual())
			eachBullet.Draw(deviceContext,basicEffect);
	}
}

void BulletSystem::removeBullet()
{
	
	for (auto bullet = mBulletVec.cbegin();bullet!=mBulletVec.cend();)
	{
		if (bullet->getVisual() == false)
		{
			bullet = mBulletVec.erase(bullet);
		}
		else 
		{
			bullet++;
		}
	}
}
