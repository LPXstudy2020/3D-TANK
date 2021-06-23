#ifndef STATE_H
#define STATE_H


#pragma once
class AITank;
template<class T>
class State
{
public:
	virtual ~State() {}
	virtual void enter(T* entity) = 0;
	virtual void execute(T* entity,float dt) = 0;
	virtual void exit(T* entity) = 0;

};


class AITankPatrolState :public State<AITank> 
{
public:
	AITankPatrolState(const AITankPatrolState&) = delete;
	AITankPatrolState& operator=(const AITankPatrolState&) = delete;

	static AITankPatrolState* getInstance();
	void enter(AITank* entity)override;
	void execute(AITank* entity,float dt)override;
	void exit(AITank* entity)override;
	~AITankPatrolState();
private:
	AITankPatrolState();
};

class MoveForwardState :public State<AITank>
{
public:
	MoveForwardState(const MoveForwardState&) = delete;
	MoveForwardState& operator=(const MoveForwardState&) = delete;

	static MoveForwardState* getInstance();
	void enter(AITank* entity)override;
	void execute(AITank* entity, float dt)override;
	void exit(AITank* entity)override;
	~MoveForwardState();
private:
	MoveForwardState();
};

class MoveBackState :public State<AITank>
{
public:
	MoveBackState(const MoveBackState&) = delete;
	MoveBackState& operator=(const MoveBackState&) = delete;

	static MoveBackState* getInstance();
	void enter(AITank* entity)override;
	void execute(AITank* entity, float dt)override;
	void exit(AITank* entity)override;
	~MoveBackState();
private:
	MoveBackState();
};




class AITankBattleState :public State<AITank>
{
public:
	AITankBattleState(const AITankBattleState&) = delete;
	AITankBattleState& operator=(const AITankBattleState&) = delete;

	static AITankBattleState* getInstance();
	void enter(AITank* entity)override;
	void execute(AITank* entity,float dt)override;
	void exit(AITank* entity)override;
	~AITankBattleState();
private:
	AITankBattleState();
};

#endif // 
