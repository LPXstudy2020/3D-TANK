#include "State.h"
#include "AItank.h"
// -------------------------------- AITankPatrolState -----------------------------
AITankPatrolState::AITankPatrolState() 
{

}

AITankPatrolState* AITankPatrolState::getInstance() 
{
	static AITankPatrolState instance;
	return &instance;
}

void AITankPatrolState::enter(AITank* entity) {
	
}
void AITankPatrolState::execute(AITank* entity,float dt)
{
	entity->patrolForwardBack(dt,5.0f);
	if (entity->findEnemy()) 
	{
		entity->lockTarget(dt);
		entity->changeState(AITankBattleState::getInstance());
	}
}
void AITankPatrolState::exit(AITank* entity)
{

}
AITankPatrolState::~AITankPatrolState() 
{

}



//--------------------------AITankBattleState -------------------------------
AITankBattleState::AITankBattleState()
{

}

AITankBattleState* AITankBattleState::getInstance()
{
	static AITankBattleState instance;
	return &instance;
}

void AITankBattleState::enter(AITank* entity) {

}
void AITankBattleState::execute(AITank* entity,float dt)
{
	entity->lockTarget(dt);
	entity->rotateToEnemy(dt);
	if (entity->getDistanceWithTarget() < entity->getAttackMinDistance()) 
		entity->changeState(MoveBackState::getInstance());
	if(entity->getDistanceWithTarget() < entity->getAttackDistance())
		entity->attackEnemy(dt);
	else
		entity->changeState(MoveForwardState::getInstance());
	
}
void AITankBattleState::exit(AITank* entity)
{

}
AITankBattleState::~AITankBattleState()
{

}

// ---------------------MoveForwardState and MoveBackState -------------------------------
MoveForwardState::MoveForwardState()
{

}

MoveForwardState* MoveForwardState::getInstance()
{
	static MoveForwardState instance;
	return &instance;
}

void MoveForwardState::enter(AITank* entity) {

}
void MoveForwardState::execute(AITank* entity, float dt)
{
	entity->lockTarget(dt);
	entity->rotateToEnemy(dt);

	if (entity->getDistanceWithTarget() > entity->getAttackDistance())
		entity->moveUp(dt);
	else
		entity->changeState(AITankBattleState::getInstance());

}
void MoveForwardState::exit(AITank* entity)
{

}
MoveForwardState::~MoveForwardState()
{

}


MoveBackState::MoveBackState()
{

}

MoveBackState* MoveBackState::getInstance()
{
	static MoveBackState instance;
	return &instance;
}

void MoveBackState::enter(AITank* entity) {

}
void MoveBackState::execute(AITank* entity, float dt)
{
	entity->lockTarget(dt);
	entity->rotateToEnemy(dt);

	if (entity->getDistanceWithTarget() < entity->getAttackMinDistance())
		entity->moveDown(dt);
	else
		entity->changeState(AITankBattleState::getInstance());

}
void MoveBackState::exit(AITank* entity)
{

}
MoveBackState::~MoveBackState()
{

}