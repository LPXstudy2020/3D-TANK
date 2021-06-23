#include "Tank.h"

using namespace DirectX;
Tank::Tank()
{
	//坦克
	tankBattery.m_ObjReader.Read(L"Resource\\Tank\\TankBattery.mbo",L"Resource\\Tank\\TankBattery.OBJ");
	tankBody.m_ObjReader.Read(L"Resource\\Tank\\TankBody.mbo",L"Resource\\Tank\\TankBody.OBJ");
	tankTrack_L.m_ObjReader.Read(L"Resource\\Tank\\TankTrack_L.mbo",L"Resource\\Tank\\TankTrack_L.OBJ");
	tankTrack_R.m_ObjReader.Read(L"Resource\\Tank\\TankTrack_R.mbo",L"Resource\\Tank\\TankTrack_R.OBJ");
	//子弹
	bullet.m_ObjReader.Read(L"TestModel\\Ball.mbo", L"TestModel\\Ball.obj");
	setHp(100);
}

void Tank::initTank(ID3D11Device* device)
{
	tankBody.SetModel(Model(device,tankBody.m_ObjReader));
	tankBattery.SetModel(Model(device, tankBattery.m_ObjReader));
	tankTrack_L.SetModel(Model(device, tankTrack_L.m_ObjReader));
	tankTrack_R.SetModel(Model(device, tankTrack_R.m_ObjReader));
	//缩放尺寸
	setScale(0.015f, 0.015f, 0.015f);
	// 让坦克底部紧贴地面
	//setRotationAll(XMFLOAT3(0.0f,XM_PI,0.0f));
	
	//子弹初始化
	bullet.SetModel(Model(device,bullet.m_ObjReader));
	//XMFLOAT3 position = tankBattery.GetTransform().GetPosition();
	//position.y += 2.0f;
	//position.z -= 3.0f;
	//bullet.setPosition(position);
	bullet.setScale(0.2f,0.2f,0.2f);
}

void Tank::setPositon(float x, float y, float z)
{
	this->GetTransform().SetPosition(x,y,z);
	tankBattery.GetTransform().SetPosition(x, y, z);
	tankBody.GetTransform().SetPosition(x, y, z);
	tankTrack_L.GetTransform().SetPosition(x, y, z);
	tankTrack_R.GetTransform().SetPosition(x, y, z);
}
void Tank::setPosition(const DirectX::XMFLOAT3 & position)
{
	this->GetTransform().SetPosition(position);
	tankBattery.GetTransform().SetPosition(position);
	tankBody.GetTransform().SetPosition(position);
	tankTrack_L.GetTransform().SetPosition(position);
	tankTrack_R.GetTransform().SetPosition(position);
}
void Tank::setRotationAll (const DirectX::XMFLOAT3& eulerAngle)
{
	this->GetTransform().SetRotation(eulerAngle);
	tankBattery.GetTransform().SetRotation(eulerAngle);
	tankBody.GetTransform().SetRotation(eulerAngle);
	tankTrack_L.GetTransform().SetRotation(eulerAngle);
	tankTrack_R.GetTransform().SetRotation(eulerAngle);
}

bool Tank::isCollision(BoundingBox& box)
{

	if (tankBox.Intersects(box))
	{
		return true;
	}
	else
	return false;
}
void Tank::solveCollision(GameObject * obj,float dt)
{
	if (obj!=NULL&&GetBoundingBox().Intersects(obj->GetBoundingBox()))
	{

		if (mDirection == Tank::Tankdirection::up)
		{
				moveDown(dt);
		}
		else if (mDirection == Tank::Tankdirection::down)
		{
				moveUp(dt);
		}
		else if (mDirection == Tank::Tankdirection::left)
		{
			moveDown(dt);
		}
		else if (mDirection == Tank::Tankdirection::right)
		{
			moveDown(dt);
		}
		
	}
}
BoundingBox Tank::GetBoundingBox() const
{
	//包围盒移动
	BoundingBox box;
	BoundingBox::CreateMerged(box, tankTrack_R.GetBoundingBox(), tankTrack_L.GetBoundingBox());
	BoundingBox::CreateMerged(box, box, tankBody.GetBoundingBox());
	return box;
}
void Tank::Draw(ID3D11DeviceContext * deviceContext, BasicEffect & effect)
{
	if (getHp() > 0)
	{
		tankBattery.Draw(deviceContext, effect);
		tankBody.Draw(deviceContext, effect);
		tankTrack_L.Draw(deviceContext, effect);
		tankTrack_R.Draw(deviceContext, effect);
	}
}

DirectX::XMFLOAT3 Tank::getPosition()
{
	return GetTransform().GetPosition();
}
void Tank::setScale(float x, float y, float z)
{
	XMMATRIX S = XMMatrixScaling(x, y, z);
	this->GetTransform().SetScale(x, y, z);
	tankBattery.GetTransform().SetScale(x, y, z);
	tankBody.GetTransform().SetScale(x, y, z);
	tankTrack_L.GetTransform().SetScale(x, y, z);
	tankTrack_R.GetTransform().SetScale(x, y, z);
	// 获取坦克包围盒
	//包围盒也缩小
	tankBodyBox = tankBody.GetLocalBoundingBox();
	tankBodyBox.Transform(tankBodyBox, S);
	tankBatteryBox = tankBattery.GetLocalBoundingBox();
	tankBatteryBox.Transform(tankBatteryBox, S);
	tankTrack_LBox = tankTrack_L.GetLocalBoundingBox();
	tankTrack_LBox.Transform(tankTrack_LBox, S);
	tankTrack_RBox = tankTrack_R.GetLocalBoundingBox();
	tankTrack_RBox.Transform(tankTrack_RBox, S);
}



void Tank::moveUp(float dt)
{
	//mDirection = up;
	XMVECTOR rightVec = XMLoadFloat3(&GetTransform().GetRightAxis());
	XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
	XMFLOAT3 front;
	XMStoreFloat3(&front, frontVec);
	float distance = walkSpeed * -dt;
	GetTransform().Translate(front, distance);
	tankBody.GetTransform().Translate(front, distance);
	tankBattery.GetTransform().Translate(front, distance);
	tankTrack_L.GetTransform().Translate(front, distance);
	tankTrack_R.GetTransform().Translate(front, distance);

	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(XMLoadFloat3(&GetTransform().GetPosition()), 
		XMVectorSet(-148.0f, 0.0f, -148.0f, 0.0f), XMVectorReplicate(148.0f)));
	setPosition(adjustedPos);
}

void Tank::moveDown(float dt)
{
	//mDirection = down;

	XMVECTOR rightVec = XMLoadFloat3(&GetTransform().GetRightAxis());
	XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
	XMFLOAT3 front;
	XMStoreFloat3(&front, frontVec);
	float distance = walkSpeed * dt;

	GetTransform().Translate(front, distance);
	tankBody.GetTransform().Translate(front, distance);
	tankBattery.GetTransform().Translate(front, distance);
	tankTrack_L.GetTransform().Translate(front, distance);
	tankTrack_R.GetTransform().Translate(front, distance);

	XMFLOAT3 adjustedPos;
	XMStoreFloat3(&adjustedPos, XMVectorClamp(XMLoadFloat3(&GetTransform().GetPosition()), XMVectorSet(-148.0f, 0.0f, -148.0f, 0.0f), XMVectorReplicate(148.0f)));
	setPosition(adjustedPos);
}

void Tank::turnLeft(float radian)
{
	//mDirection = left;

	XMFLOAT3 rotation = GetTransform().GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + -radian * rotateYSpeedTankBody);
	GetTransform().SetRotation(rotation);
	tankBody.GetTransform().SetRotation(rotation);
	tankTrack_L.GetTransform().SetRotation(rotation);
	tankTrack_R.GetTransform().SetRotation(rotation);
	

}

void Tank::turnRight(float radian)
{
	//mDirection = right;

	XMFLOAT3 rotation = GetTransform().GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + radian * rotateYSpeedTankBody);
	GetTransform().SetRotation(rotation);
	tankBody.GetTransform().SetRotation(rotation);
	tankTrack_L.GetTransform().SetRotation(rotation);
	tankTrack_R.GetTransform().SetRotation(rotation);

}

void Tank::rotateLeftRightSight(float radian )
{

		XMFLOAT3 rotation = tankBattery.GetTransform().GetRotation();
		rotation.y = XMScalarModAngle(radian * rotateYSpeedTankBattery + rotation.y); //把旋转角度 标准化 限制在 [-PI/2,PI/2] 
		tankBattery.GetTransform().SetRotation(rotation);
}


void Tank::turnUpDownSight(float radian)
{
	XMFLOAT3 rotation = tankBattery.GetTransform().GetRotation();
	rotation.x = XMScalarModAngle(radian*rotateXSpeedTankBattery + rotation.x); //把旋转角度 标准化 限制在 [-PI/2,PI/2] 
	//limit 角度问题
	if (rotation.x >= DomainOfTankBattery[1])
	{
		rotation.x = DomainOfTankBattery[1];
	}
	else if (rotation.x <= DomainOfTankBattery[0]) {
		rotation.x = DomainOfTankBattery[0];
	}
	tankBattery.GetTransform().SetRotation(rotation);
}


void Tank::fire(BulletSystem& bulletSystem)
{
	//bullet.changeModel(Model());
	XMFLOAT3 position = tankBattery.GetTransform().GetPosition();
	position.y += 2.0f;

	bullet.setPosition(position);
	XMFLOAT3 direction = tankBattery.GetTransform().GetForwardAxis();	
	bullet.setDirection(direction);
	bullet.GetTransform().Translate(direction,-7.0f);
	bulletSystem.addBullet(bullet);
}
void Tank::setRotationTankBase(const DirectX::XMFLOAT3& eulerAngle)
{
	GetTransform().SetRotation(eulerAngle);
	tankBody.GetTransform().SetRotation(eulerAngle);
	tankTrack_L.GetTransform().SetRotation(eulerAngle);
	tankTrack_R.GetTransform().SetRotation(eulerAngle);
}

void Tank::rotateTankBase(const DirectX::XMFLOAT3& eulerAngle) 
{
	GetTransform().Rotate(eulerAngle);
	tankBody.GetTransform().Rotate(eulerAngle);
	tankTrack_L.GetTransform().Rotate(eulerAngle);
	tankTrack_R.GetTransform().Rotate(eulerAngle);
}










void Tank::changeLenState() 
{
	lenState = !lenState;
}
void Tank::closeLen() 
{
	lenState = false;
}

bool Tank::isOpenLen() 
{
	return lenState;
}