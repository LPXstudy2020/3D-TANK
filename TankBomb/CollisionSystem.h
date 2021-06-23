#pragma once
//#include"GameObject.h"
#include"BulletSystem.h"
#include "Tank.h"
class Tank;
class CollisionSystem
{
public:

	void virtual CollisionInWorld(std::vector<Tank*> mworldObj,float dt,BulletSystem& bulletSystem);

	std::vector<Tank*> getWorldObj();
	std::vector<GameObject*> getOtherObj();
	void setWorldObject(Tank* obj);
	void setWorldOtherObject(GameObject* obj);
	
private:
	//需要检测的碰撞物体
	std::vector<Tank*> worldObj;
	//不需要检测碰撞的物体
	std::vector<GameObject*> otherObj;
};

