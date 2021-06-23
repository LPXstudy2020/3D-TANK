#ifndef AITANK_H
#define AITANK_H

#pragma once
#include"Tank.h"



const float PIDIV8 = 0.39269908f;
template<class AITank> class State;
class BulletSystem;
class AITank:public Tank
{
public:

	AITank();
	~AITank();
	virtual void onHide();
	//开火
	//void fire(BulletSystem& bulletSystem);
	
	//获取tank包围盒
	DirectX::BoundingBox GetBoundingBox() const override;
	//更新Tank
	void update(float dt);
	//更新AITank
	void updateAI(float d, DirectX::BoundingBox& box);
	Bullet bullet;
	bool findEnemy();
	void lockTarget(float dt);
	void setTarget(GameObject* target);
	void patrolForwardBack(float dt,float distance);
	void patrolAroundCircle(float dt,float radius);
	void setBulletSystem(BulletSystem* bulletSystem);
	void rotateToEnemy(float dt);
	void changeState(State<AITank>* newState);
	void attackEnemy(float dt);
	float getDistanceWithTarget();
	float getAttackDistance();
	float getAttackMinDistance();
	void aimToTarget();
private:
	State<AITank>* mCurrentState = nullptr;
	float watchDistance = 40.0f;
	float watchRadias = PIDIV8;
	float attackDistance = 50.0f;
	float attackInterval = 4.0f;
	float targetDistance = 0.0f;
	float attackMinDistance = 25.0f;

	enum class MoveState { FORWARD, BACK, IDLE };
	enum class TurnState { LEFT, RIGHT, IDLE };
	enum class SightState { UP, DOWN, LEFT, RIGHT, IDLE };
	MoveState mMoveState = MoveState::IDLE;
	TurnState mTurnState = TurnState::IDLE;
	SightState mSightState = SightState::IDLE;
	BulletSystem* mBulletSystem;
	GameObject* mTarget;
	


};

#endif