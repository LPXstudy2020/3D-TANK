#include "PlayController.h"

using namespace DirectX;



PlayController::PlayController():m_CameraMode(CameraMode::ThirdPerson)
{
}

void PlayController::initCamera(std::shared_ptr<Camera>& camera)
{
	cam3rd = std::dynamic_pointer_cast<ThirdPersonCamera>(camera);
	cam1st = std::dynamic_pointer_cast<FirstPersonCamera>(camera);
}

void PlayController::changeCamera()
{
	
}
void PlayController::setMouseState()
{

	mouseState = Mouse::Get().GetState();
	mMouseTracker.Update(mouseState);
}

void PlayController::mousePress(float dt, BasicEffect& m_BasicEffect,
	std::shared_ptr<Camera>& camera, Tank& tank,BulletSystem& bulletSystem)
{
	
	setMouseState();
	if (m_CameraMode == CameraMode::FirstPerson)
	{
		if (mouseState.positionMode == Mouse::MODE_RELATIVE)
		{
			//镜头的朝向改变
			cam1st->domainPitch[0] = 0.0f;
			cam1st->domainPitch[1] = XM_PI / 4;
			cam1st->Pitch(mouseState.y * dt * 3.0f); //抬头低头
			cam1st->RotateY(mouseState.x * dt * 3.0f);


			//tank炮筒朝向改变
			tank.turnUpDownSight(-mouseState.y * dt);//抬头低头
			tank.rotateLeftRightSight(mouseState.x * dt);//左右移动
		}
		XMFLOAT3 pos = tank.tankBattery.GetTransform().GetPosition();
		pos.y += 2.0f;
		XMFLOAT3 target = tank.tankBattery.GetTransform().GetForwardAxis();
		target.x = -target.x;
		target.y = -target.y;
		target.z = -target.z;

		//射击
		if (mMouseTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) 
		{
			tank.fire(bulletSystem);
		}

		//开镜状态
		if (mMouseTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
		{

			tank.changeLenState();
		}
		if (tank.isOpenLen())
		{
			cam1st->SetFrustum(XM_PI / 20, 1.4f, 0.5f, 1000.0f);
			m_BasicEffect.SetProjMatrix(camera->GetProjXM());
		}
		else
		{
			cam1st->SetFrustum(XM_PI / 3, 1.4f, 0.5f, 1000.0f);
			m_BasicEffect.SetProjMatrix(camera->GetProjXM());
		}
		cam1st->LookTo(pos, target, XMFLOAT3(0.0f, 1.0f, 0.0f));
	}	else if (m_CameraMode == CameraMode::ThirdPerson)
	{
		cam3rd->SetTarget(tank.getPosition());

		if (mouseState.positionMode == Mouse::MODE_RELATIVE)
		{
			cam3rd->RotateX(mouseState.y * dt * 1.25f);
			cam3rd->RotateY(mouseState.x * dt * 1.25f);
			cam3rd->Approach(-mouseState.scrollWheelValue / 120 * 1.0f);
		}
	}

}

 void PlayController::setKeyboardState()
{
	 keyState = Keyboard::Get().GetState();
	 mKeyboardTracker.Update(keyState);
}

 void PlayController::keyPress(float dt, std::shared_ptr<Camera>& camera,Tank& tank, BasicEffect& m_BasicEffect)
 {
	 setKeyboardState();
	 //设置Tank移动模式 WASD 上下左右
	// 机身移动转向，直行
	 if (keyState.IsKeyDown(Keyboard::W))
	 {
		 tank.mDirection = Tank::Tankdirection::up;
		 tank.moveUp(dt);
	 }
	 if (keyState.IsKeyDown(Keyboard::S))
	 {
		 tank.mDirection = Tank::Tankdirection::down;
		 tank.moveDown(dt);
	 }
	 if (keyState.IsKeyDown(Keyboard::A))
	 {
		 tank.mDirection = Tank::Tankdirection::left;
		 tank.turnLeft(dt);
	 }
	 if (keyState.IsKeyDown(Keyboard::D))
	 {
		 tank.mDirection = Tank::Tankdirection::right;
		 tank.turnRight(dt);
	 }
	 // 机炮移动方向移动
	 if (keyState.IsKeyDown(Keyboard::Left))
	 {
		 tank.rotateLeftRightSight(-dt);
	 }
	 if (keyState.IsKeyDown(Keyboard::Right))
	 {
		 tank.rotateLeftRightSight(dt);
	 }

	 if (keyState.IsKeyDown(Keyboard::Up))
	 {
		 tank.turnUpDownSight(dt);
	 }
	 if (keyState.IsKeyDown(Keyboard::Down))
	 {
		 tank.turnUpDownSight(-dt);
	 }
	 if (mKeyboardTracker.IsKeyPressed(Keyboard::D1) && m_CameraMode != CameraMode::FirstPerson)
	 {
		 if (!cam1st)
		 {
			 cam1st.reset(new FirstPersonCamera);
			 cam1st->SetFrustum(XM_PI / 3, 1.333f, 0.5f, 1000.0f);
			 camera = cam1st;
		 }
		 XMFLOAT3 pos = tank.tankBattery.GetTransform().GetPosition();
		 pos.y += 2.0f;
		 XMFLOAT3 target = tank.tankBattery.GetTransform().GetForwardAxis();
		 target.x = -target.x;
		 target.y = -target.y;
		 target.z = -target.z;
		 cam1st->LookTo(pos, target, XMFLOAT3(0.0f, 1.0f, 0.0f));

		 m_CameraMode = CameraMode::FirstPerson;
	 }
	 else if (mKeyboardTracker.IsKeyPressed(Keyboard::D2) && m_CameraMode != CameraMode::ThirdPerson)
	 {
		 tank.closeLen();
		 if (!cam3rd)
		 {
			 cam3rd.reset(new ThirdPersonCamera);
			 cam3rd->SetFrustum(XM_PI / 3, 1.333f, 0.5f, 1000.0f);
			 camera = cam3rd;
			 m_BasicEffect.SetProjMatrix(camera->GetProjXM());
		 }
		 XMFLOAT3 target = tank.getPosition();
		 cam3rd->SetTarget(target);
		 cam3rd->SetDistance(10.0f);
		 cam3rd->SetDistanceMinMax(2.0f, 100.0f);
		 cam3rd->SetRotationX(XM_PIDIV2);
		 m_CameraMode = CameraMode::ThirdPerson;
	 }

 }

 void PlayController::quitWindow(HWND mWnd) 
 {
	 if (mKeyboardTracker.IsKeyPressed(Keyboard::Escape))
		 SendMessage(mWnd, WM_DESTROY, 0, 0);
 }
