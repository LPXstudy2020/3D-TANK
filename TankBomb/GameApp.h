#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjReader.h"
#include "Tank.h"
#include "SkyRender.h"
#include"AITank.h"
#include "PlayController.h"
#include "BulletSystem.h"
#include "UI.h"
#include"CollisionSystem.h"
class GameApp : public D3DApp
{
public:
	// 摄像机模式
	//enum class CameraMode { FirstPerson, ThirdPerson, FirstPersonAim};

public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitResource();
	//void CreateRandomTrees(); create trees
private:

	UiManager uiManager;

	Tank tank;                                                  //坦克

	GameObject m_Ground;										// 地面

	std::vector<GameObject> ground;
	GameObject mWaterTank;
	GameObject mWaterTank2;
	GameObject mWaterTank3;
	std::vector<GameObject> walls;							// 墙壁
	BulletSystem bulletSystem;
	AITank aiTank_01;                           //aitank
	AITank aiTank_02;
	AITank aiTank_03;

	SkyEffect m_SkyEffect;									    // 天空盒特效管理
	std::unique_ptr<SkyRender> m_pDaylight;					    // 天空盒(白天)

	BasicEffect m_BasicEffect;							        // 对象渲染特效管理

	std::shared_ptr<Camera> m_pCamera;						    // 摄像机
	//CameraMode m_CameraMode;									// 摄像机模式
	PlayController playController;
	ObjReader m_ObjReader;									    // 模型读取对象
	PointLight pointLight;

	CollisionSystem collisionSystem;
	
};


#endif