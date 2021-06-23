#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	uiManager.init();

	if (!D3DApp::Init())
		return false;

	// 务必先初始化所有渲染状态，以供下面的特效使用
	RenderStates::InitAll(m_pd3dDevice.Get());

	if (!m_BasicEffect.InitAll(m_pd3dDevice.Get()))
		return false;

	if (!m_SkyEffect.InitAll(m_pd3dDevice.Get()))
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要

	Mouse::Get().SetWindow(m_hMainWnd);
	Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);

	
	return true;
}

void GameApp::OnResize()
{
	uiManager.releaseResources();

	D3DApp::OnResize();

	uiManager.obtainResources(m_pSwapChain.Get(), m_ClientWidth, m_ClientHeight);


	
	// 摄像机变更显示
	if (m_pCamera != nullptr)
	{
		m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
		m_pCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
		m_BasicEffect.SetProjMatrix(m_pCamera->GetProjXM());
	}
}

void GameApp::UpdateScene(float dt)
{

	//初始化摄像机 
	playController.initCamera(m_pCamera);

	//鼠标按压事件
	playController.mousePress(dt,m_BasicEffect, m_pCamera,tank,bulletSystem);
	//键盘按压事件
	playController.keyPress(dt,m_pCamera,tank,m_BasicEffect);

	m_BasicEffect.SetViewMatrix(m_pCamera->GetViewXM());
    m_BasicEffect.SetEyePos(m_pCamera->GetPositionXM());
    // 重置滚轮值211
     m_pMouse->ResetScrollWheelValue();

	aiTank_01.update(dt);
	aiTank_02.update(dt);
	aiTank_03.update(dt);
	 //碰撞检测
	collisionSystem.CollisionInWorld(collisionSystem.getWorldObj(), dt,bulletSystem);
	bulletSystem.update(dt);
	XMFLOAT3 position = tank.tankBody.GetTransform().GetPosition();
	position.y += 20.0f;
	pointLight.position = position;
	m_BasicEffect.SetPointLight(0, pointLight);
	//ECS 退出程序
	playController.quitWindow(MainWnd());

}

//每一次都需要绘制的图形
void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	// ******************
	// 绘制Direct3D部分
	//
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_BasicEffect.SetRenderDefault(m_pd3dImmediateContext.Get(), BasicEffect::RenderObject);
	m_BasicEffect.Apply(m_pd3dImmediateContext.Get());
	//地板

	m_Ground.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	mWaterTank.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	mWaterTank2.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	mWaterTank3.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);

	//墙体
	for (int i = 0; i < 4; ++i)
	{
		walls[i].Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	}
	//aiTank
	tank.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);

	aiTank_01.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	aiTank_02.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
	aiTank_03.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);

	bulletSystem.draw(m_pd3dImmediateContext.Get(), m_BasicEffect);

	// 绘制天空盒
	m_SkyEffect.SetRenderDefault(m_pd3dImmediateContext.Get());
	m_pDaylight->Draw(m_pd3dImmediateContext.Get(), m_SkyEffect, *m_pCamera);

	uiManager.render();
	HR(m_pSwapChain->Present(0, 0));
}



bool GameApp::InitResource()
{

	// 初始化天空盒
	m_pDaylight = std::make_unique<SkyRender>();
	HR(m_pDaylight->InitResource(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get(),
		L"Resource\\Objects\\daylight.jpg",
		5000.0f));
	// ******************
	// 初始化游戏对象
	//



	ObjReader objReader;
	//水箱
	objReader.Read(L"Resource\\Objects\\SM_WaterTank_01a.mbo",L"Resource\\Objects\\SM_WaterTank_01a.OBJ");
	mWaterTank.SetModel(Model(m_pd3dDevice.Get(), objReader));
	mWaterTank.GetTransform().SetPosition(0.0f,0.0f,20.0f);
	mWaterTank.GetTransform().SetScale(0.020f, 0.02f, 0.02f);
	collisionSystem.setWorldOtherObject(&mWaterTank);
	//水箱
	objReader.Read(L"Resource\\Objects\\SM_Crate_01a.mbo",L"Resource\\Objects\\SM_Crate_01a.OBJ");
	mWaterTank3.SetModel(Model(m_pd3dDevice.Get(), objReader));
	mWaterTank3.GetTransform().SetPosition(0.0f, 0.0f, 40.0f);
	mWaterTank3.GetTransform().SetScale(0.020f, 0.02f, 0.02f);
	collisionSystem.setWorldOtherObject(&mWaterTank3);
	//水箱
	objReader.Read(L"Resource\\Objects\\SM_FreightContainer_01.mbo",L"Resource\\Objects\\SM_FreightContainer_01.OBJ");
	mWaterTank2.SetModel(Model(m_pd3dDevice.Get(), objReader));
	mWaterTank2.GetTransform().SetPosition(0.0f, 0.0f, 60.0f);
	mWaterTank2.GetTransform().SetScale(0.020f, 0.02f, 0.02f);
	collisionSystem.setWorldOtherObject(&mWaterTank2);


	//墙体
	walls.resize(4);
	walls[0].m_ObjReader.Read(L"Resource\\Objects\\SM_construction_fence_01a.mbo", L"Resource\\Objects\\SM_construction_fence_01a.OBJ");
	for (int i = 0; i < 4; ++i)
	{
		walls[i].SetModel(Model(m_pd3dDevice.Get(), walls[0].m_ObjReader));
		walls[i].GetTransform().SetScale(1.0f, 0.03f, 1.0f);
		
	}
	walls[0].GetTransform().SetPosition(150.0f, -1.0f, -150.0f);
	walls[0].GetTransform().SetRotation(0.0f, XM_PIDIV2, 0.0f);

	walls[1].GetTransform().SetPosition(-150.0f, -1.0f, -150.0f);
	walls[1].GetTransform().SetRotation(0.0f, XM_PIDIV2, 0.0f);

	walls[2].GetTransform().SetPosition(-150.0f, -1.0f, -150.0f);
	walls[2].GetTransform().SetRotation(0.0f, XM_PIDIV2*2, 0.0f);

	walls[3].GetTransform().SetPosition(-150.0f, -1.0f, 150.0f);
	walls[3].GetTransform().SetRotation(0.0f, XM_PIDIV2 * 2, 0.0f);



	// 初始化地形1
	
	m_Ground.m_ObjReader.Read(L"TestModel\\ground.mbo", L"TestModel\\ground.OBJ");

	m_Ground.SetModel(Model(m_pd3dDevice.Get(), m_Ground.m_ObjReader));
	m_Ground.GetTransform().SetPosition(0.0f, 1.0f, 0.0f);
	m_Ground.GetTransform().SetScale(15.0f, 1.0f, 15.0f);
	collisionSystem.setWorldOtherObject(&m_Ground);




	// 初始化坦克
	tank.initTank(m_pd3dDevice.Get());
	// 初始化坦克ai
	aiTank_01.initTank(m_pd3dDevice.Get());
	aiTank_01.setTarget(&tank);
	aiTank_01.setBulletSystem(&bulletSystem);
	aiTank_02.initTank(m_pd3dDevice.Get());
	aiTank_02.setTarget(&tank);
	aiTank_02.setBulletSystem(&bulletSystem);
	aiTank_03.initTank(m_pd3dDevice.Get());
	aiTank_03.setTarget(&tank);
	aiTank_03.setBulletSystem(&bulletSystem);
	//碰撞系统
	collisionSystem.setWorldObject(&tank);
	collisionSystem.setWorldObject(&aiTank_01);
	collisionSystem.setWorldObject(&aiTank_02);
	collisionSystem.setWorldObject(&aiTank_03);


	// ******************
	// 初始化摄像机
	//

	auto camera = std::shared_ptr<ThirdPersonCamera>(new ThirdPersonCamera);
	m_pCamera = camera;
	
	camera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
	camera->SetTarget(XMFLOAT3(0.0f, 0.5f, 0.0f));
	camera->SetDistance(10.0f);
	camera->SetDistanceMinMax(3.0f, 100.0f);
	camera->SetRotationX(XM_PIDIV4);
	camera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);

	m_BasicEffect.SetWorldMatrix(XMMatrixIdentity());
	m_BasicEffect.SetViewMatrix(camera->GetViewXM());
	m_BasicEffect.SetProjMatrix(camera->GetProjXM());
	m_BasicEffect.SetEyePos(camera->GetPositionXM()); //用在PS上,针对光源
	
	// ******************
	// 初始化不会变化的值
	//

	// 环境光
	DirectionalLight dirLight;
	dirLight.ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	dirLight.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_BasicEffect.SetDirLight(0, dirLight);
	// 灯光

	pointLight.position = XMFLOAT3(0.0f, 20.0f, 0.0f);
	pointLight.ambient = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	pointLight.range = 26.0f;	
	m_BasicEffect.SetPointLight(0, pointLight);


	// ******************
	// 设置调试对象名
	//
	//m_Ground.SetDebugObjectName("Ground");
	//tank.SetDebugObjectName("House");


	return true;
}





