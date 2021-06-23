#include "AITank.h"
#include "keyboard.h"
#include "State.h"

using namespace DirectX;
AITank::AITank()
{
	setScale(0.015f, 0.015f, 0.015f);  // 缩小物体;
	srand(time(0));
	int random;
	random = rand() % 10 + 5;
	setPositon(8.0f*random, 0.0f, 10.0f*random);
	
	BoundingBox box;
	BoundingBox::CreateMerged(box, tankTrack_R.GetBoundingBox(), tankTrack_L.GetBoundingBox());
	BoundingBox::CreateMerged(tankBox, box, tankBody.GetBoundingBox());
	mCurrentState = AITankPatrolState::getInstance();
	setHp(50);
}

AITank::~AITank()
{

}

void AITank::onHide()
{
}



//bool AITank::isCollision(DirectX::BoundingBox & box)
//{
//	if (AItank.GetBoundingBox.Intersects(box))
//	{
//		return true;
//	}
//	else
//		return false;
//	return false;
//}

DirectX::BoundingBox AITank::GetBoundingBox() const
{
	//包围盒移动

	BoundingBox box;
	BoundingBox::CreateMerged(box, tankBattery.GetBoundingBox(), tankTrack_L.GetBoundingBox());
	BoundingBox::CreateMerged(box, box, tankTrack_R.GetBoundingBox());
	return box;
}

void AITank::update(float dt)
{
	
	if (getHp() > 0)
	{


		mCurrentState->execute(this, dt);
	}
}
void AITank::setBulletSystem(BulletSystem* bulletSystem) 
{
	mBulletSystem = bulletSystem;
}
void AITank::setTarget(GameObject* target) 
{
	mTarget = target;
}

bool AITank::findEnemy()
{
	//AI 到目标tank的距离是否超过AI的视线范围
	XMFLOAT3 playerPosition = mTarget->GetTransform().GetPosition();
	XMFLOAT3 myPosition = GetTransform().GetPosition();
	
	XMVECTOR playerVec = XMLoadFloat3(&playerPosition);
	XMVECTOR myVec = XMLoadFloat3(&myPosition);
	XMVECTOR subB = playerVec - myVec;
	float distanceSq = XMVectorGetX(XMVector3LengthSq(subB));
	targetDistance = sqrt(distanceSq);
	if (distanceSq < watchDistance * watchDistance) 
	{
		//AI与目标角度是否在AI的视线角度中，使用dot
		XMFLOAT3 forWard = tankBattery.GetTransform().GetForwardAxis();
		forWard.x = -forWard.x;
		forWard.y = -forWard.y;
		forWard.z = -forWard.z;


		XMVECTOR subA = XMLoadFloat3(&forWard);
		float dotValue = XMVectorGetX(XMVector3Dot(subA,subB));
		if (dotValue > 0) 
		{
			float a = XMVectorGetX(XMVector3Length(subA));
			float b = XMVectorGetX(XMVector3Length(subB));
			float cosVal = dotValue / (a * b);
			float arcCos = XMScalarACos(cosVal);
			if (arcCos < watchRadias) 
			{
				return true;
			}
		}

	}

	return false;
}

void AITank::lockTarget(float dt) 
{
	//AI 到目标tank的距离是否超过AI的视线范围
	XMFLOAT3 playerPosition = mTarget->GetTransform().GetPosition();
	XMFLOAT3 myPosition = tankBattery.GetTransform().GetPosition();
	XMFLOAT3 myLastRotation = tankBattery.GetTransform().GetRotation();
	XMVECTOR playerVec = XMLoadFloat3(&playerPosition);
	XMVECTOR myVec = XMLoadFloat3(&myPosition);
	XMVECTOR sub2 =myVec - playerVec;
	XMFLOAT3 direction;
	float distance = XMVectorGetX(XMVector3Length(sub2));
	targetDistance = distance;
	XMStoreFloat3(&direction,sub2);
	this->tankBattery.GetTransform().LookTo(direction);

	XMFLOAT3 myCurrRotation = tankBattery.GetTransform().GetRotation();
	
	if (myLastRotation.y < 0) 
	{
		myLastRotation.y += XM_2PI;
	}
	if (myCurrRotation.y < 0||myCurrRotation.y < myLastRotation.y)
	{
		myCurrRotation.y += XM_2PI;
	}
	float substr = myCurrRotation.y - myLastRotation.y;
	myLastRotation.y += substr * dt;


	float val = bullet.getFallSpeed()*targetDistance / (bullet.getSpeed()*bullet.getSpeed());
	float value = 0.5* XMScalarASin(val) ;
	//float radian = DirectX::XM_PIDIV2 - value;
	
	float diff = value - myLastRotation.x;

	myLastRotation.x = value;

	//attackDistance = bullet.getSpeed()*bullet.getSpeed() / bullet.getFallSpeed();
	/*diff = myLastRotation.x - value;
	if (diff < 0.00001f && diff > -0.00001f) 
	{
		myLastRotation.x = value;
	}*/
	tankBattery.GetTransform().SetRotation(myLastRotation);

}
void AITank::updateAI(float dt, DirectX::BoundingBox& box)
{
	//获取当前AI和玩家的位置
	//检测当前AI和玩家的位置
	//移动，检测障碍物，一定机率旋转方向
	if (getHp() <= 0)
	{

	}
	moveUp(dt);
	srand((unsigned int)time(0));
	int random;
	random = rand() % 6 + 1;
	if (random == 1)
		turnLeft(dt);
	else if (random == 2)
		turnRight(-dt);
	else if (random == 3)
		rotateLeftRightSight(-dt);
	else if (random == 4)
		rotateLeftRightSight(dt);

	tankBox.CreateMerged(tankBox, tankBattery.GetBoundingBox(), tankTrack_L.GetBoundingBox());
	tankBox.CreateMerged(tankBox, tankBox, tankTrack_R.GetBoundingBox());
}

void AITank::patrolForwardBack(float dt, float distance)
{
	static float accumulateWalkDistance = 0.0f;
	static float accumulateTime = 0.0f;
	rotateLeftRightSight(dt);
	if (mMoveState == MoveState::IDLE)
	{
		accumulateTime += dt;
		if (accumulateTime >= 2.0f) 
		{
			mMoveState = MoveState::FORWARD;
		}
	}
	else if (mMoveState == MoveState::FORWARD) 
	{

		accumulateWalkDistance += dt * walkSpeed;
		if (accumulateWalkDistance <= distance)
		{
			moveUp(dt);
		}
		else 
		{
			accumulateWalkDistance = 0.0f;
			mMoveState = MoveState::BACK;
		}
		
	}
	else if (mMoveState == MoveState::BACK) 
	{
		accumulateWalkDistance += dt * walkSpeed;
		if (accumulateWalkDistance <= distance)
		{
			moveDown(dt);
		}
		else
		{
			accumulateTime = 0.0f;
			accumulateWalkDistance = 0.0f;
			mMoveState = MoveState::IDLE;
		}
	}


	

}

void AITank::rotateToEnemy(float dt)
{

	XMFLOAT3 rotation = tankBody.GetTransform().GetRotation();
	XMFLOAT3 enemyDirection = tankBattery.GetTransform().GetRotation();
	if (rotation.y < 0)
	{
		rotation.y += XM_2PI;
	}
	if (enemyDirection.y < 0 || enemyDirection.y < rotation.y)
	{
		enemyDirection.y += XM_2PI;
	}
	float slerp = enemyDirection.y - rotation.y;

	rotation.y += slerp * dt;
	setRotationTankBase(rotation);


}

void AITank::changeState(State<AITank>* newState)
{
	assert(mCurrentState!= nullptr&&newState!=nullptr);

	mCurrentState->exit(this);
	mCurrentState = newState;
	mCurrentState->enter(this);
}

void AITank::attackEnemy(float dt) 
{
	static float accumulateTime = 0.0f;
	accumulateTime += dt;
	if (accumulateTime > attackInterval)
	{
		accumulateTime = 0.0f;
		fire(*mBulletSystem);
	}

}

float AITank::getDistanceWithTarget() 
{
	return targetDistance;
}
float AITank::getAttackDistance()
{
	return attackDistance;
}
float AITank::getAttackMinDistance() 
{
	return attackMinDistance;
}

void AITank::aimToTarget() 
{
	
}

void AITank::patrolAroundCircle(float dt,float radius)
{
	turnLeft(dt/radius);
	rotateLeftRightSight(dt);
	moveUp(dt);
	
}