#ifndef COMPONENTS_H
#define COMPONENTS_H
#pragma once
#include <wrl/client.h>   //Microsoft::WRL::ComPtr<T>
#include <d3d11_1.h>	  //ID3D11Buffer..

// 添加所有要引用的库---Linker library

#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "D3DCompiler.lib")
//#pragma comment(lib, "winmm.lib")

#include "Effects.h"			//效果组件
#include "Model.h"				//Model part
#include "Transform.h"			//Transform Part





#endif
