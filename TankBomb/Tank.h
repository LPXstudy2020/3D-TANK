#pragma once
//#include "d3dApp.h"
#include <d3d11_1.h>
#include "GameObject.h"
#include "Transform.h"
#include <DirectXMath.h>
#include "BulletSystem.h" 
#include"CollisionSystem.h"
class Tank :public GameObject
{
public:
	
	Tank() ;
	void initTank(ID3D11Device* device);
	//缩放模型
	void setScale(float x, float y, float z);

	//设置位置
	void setPositon(float x,float y,float z);
	void setPosition(const DirectX::XMFLOAT3& position);
	//获取位置信息
	DirectX::XMFLOAT3 getPosition();
	//set rotation tank body
	void setRotationAll(const DirectX::XMFLOAT3& eulerAngle);
	void setRotationTankBase(const DirectX::XMFLOAT3& eulerAngle);
	void rotateTankBase(const DirectX::XMFLOAT3& eulerAngle);

	//移动tank 
	void moveUp(float d);
	void moveDown(float d);
	//左转
	void turnLeft(float d);
	//右转
	void turnRight(float d);

	//瞄准器左右旋转
	void rotateLeftRightSight(float d);
	//瞄准器上下旋转
	void turnUpDownSight(float d);
	
	//开火
	void fire(BulletSystem& bulletSystem);

	//碰撞检测
	bool virtual isCollision(DirectX::BoundingBox& box);
	//解决冲突
	virtual void solveCollision(GameObject* obj,float dt);

	//获取tank包围盒
	virtual DirectX::BoundingBox GetBoundingBox() const override;

   //绘制
	void Draw(ID3D11DeviceContext * deviceContext, BasicEffect& effect);
	

	void changeLenState();
	void closeLen();
	bool isOpenLen();
	const float distanceForOpenLen = 30;

	GameObject tankBody;		//坦克主体
	GameObject tankBattery;		//坦克炮台
	GameObject tankTrack_L;		//左履带
	GameObject tankTrack_R;		//右履带
	Bullet bullet;				//子弹

	DirectX::BoundingBox tankBox;        //坦克主题包围盒
	DirectX::BoundingBox tankBodyBox;    //主题包围盒
	DirectX::BoundingBox tankBatteryBox; //炮台包围盒
	DirectX::BoundingBox tankTrack_LBox;//左履带包围盒
	DirectX::BoundingBox tankTrack_RBox;//右履带包围盒

	enum class Tankdirection {up,down,left,right,leftsight,rightsight};
	Tankdirection mDirection;
protected:
	const float DomainOfTankBattery[2] = { 0,DirectX::XM_PI / 4 }; //炮筒的抬高低头限制
	const float walkSpeed = 15.0f;					 //直行后退
	const float rotateYSpeedTankBody = 1.5f;		 //身体旋转
	const float rotateXSpeedTankBattery = 1.5f;		 //炮筒抬高低头
	const float rotateYSpeedTankBattery = 1.5f;		 // 炮筒旋转
	bool lenState = false;

	bool visual = true;  //tank is visual
};

