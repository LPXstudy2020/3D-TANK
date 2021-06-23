#include "CollisionSystem.h"
using namespace DirectX;


void CollisionSystem::CollisionInWorld(std::vector<Tank*> worldObj,float dt,BulletSystem& bulletSystem)
{
	//循环检测每个移动物体
	for (auto& eachObject : worldObj)
	{
		for (auto& otherObject : otherObj)
		{
			//检测移动物体是否碰到静物
			eachObject->solveCollision(otherObject,dt);

		}
		for (auto& obj : worldObj)
		{
			//检测移动物体是否碰到移动的非自己的物体
			if (obj != eachObject)
			{
				eachObject->solveCollision(obj,dt);
			}
		}	
     }
	for (auto& bullet : *(bulletSystem.getBulletVes()))
	{
		for (auto& otherObject : otherObj)
		{
			if (bullet.isCollision(otherObject))
			{
				//检测移动物体是否碰到静物
				bullet.solveCollision(otherObject, dt);
			}
		}
		for (auto& eachObject : worldObj)
		{
			if (bullet.isCollision(eachObject))
			{
				//检测子弹是否喷到移动物体（坦克，子弹）
				bullet.solveCollision(eachObject, dt);
			}
		}
		
	}

}

std::vector<Tank*> CollisionSystem::getWorldObj()
{
	return worldObj;
}

std::vector<GameObject*> CollisionSystem::getOtherObj()
{
	return otherObj;
}

void CollisionSystem::setWorldObject(Tank* obj)
{
	worldObj.push_back(obj);
}

void CollisionSystem::setWorldOtherObject(GameObject * obj)
{
	otherObj.push_back(obj);
}
