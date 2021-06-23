#ifndef PLAY_CONTROLLER_H
#define PLAY_CONTROLLER_H
#pragma once
#include "Tank.h"
#include"AITank.h"
#include "Camera.h"
#include "d3dApp.h"
class PlayController
{
public:
	PlayController();
	// 摄像机模式
	enum class CameraMode { FirstPerson, ThirdPerson };
public:
	//摄像机初始化
	void initCamera(std::shared_ptr<Camera>& camera);
	//摄像机切换
	void changeCamera();
	
	
	//监听鼠标事件
	void mousePress(float dt, BasicEffect& m_BasicEffect,
		std::shared_ptr<Camera>& camera, Tank& tank,BulletSystem& bulletSystem);
	
   //监ting键盘事件
	void keyPress(float dt,std::shared_ptr<Camera>& camera, Tank& tank, BasicEffect& m_BasicEffect);
	//获取,设置当前控制的角色的坐标
	DirectX::XMFLOAT3 getPos();
	void setPos();
	//获取控制的tank
	Tank* getTank();
	//ECS 退出按键
	void quitWindow(HWND mWnd);
private:
	//更新鼠标状态
	void setMouseState();
	//更新键盘状态
	void setKeyboardState();

	std::shared_ptr<ThirdPersonCamera> cam3rd;
	std::shared_ptr<FirstPersonCamera> cam1st;
	CameraMode m_CameraMode;									// 摄像机模式
	DirectX::Mouse::State mouseState;       //鼠标状态
	DirectX::Keyboard::State keyState;

	DirectX::Mouse::ButtonStateTracker mMouseTracker;			// 鼠标状态追踪器
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;	// 键盘状态追踪器
	//Tank tank;                                                  //坦克
	//AITank aiTank;												//AI坦克

	//BasicEffect m_BasicEffect;							        // 对象渲染特效管理
};

#endif
