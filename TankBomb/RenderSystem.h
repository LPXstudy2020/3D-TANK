#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#pragma once
#include <d3d11_1.h>
#include <cassert>

#include "ECS.h"
#include "Components.h"

//#include "Effect.h"
class RenderSystem:public ECS::EntitySystem
{
public:
	RenderSystem() = default;
	explicit RenderSystem(ID3D11Device*  device) {
		init(device);
	}
	virtual ~RenderSystem() {}

	virtual void tick(ECS::World* world, float deltaTime)override 
	{
		world->each<Model, Transform>([&](ECS::Entity* ent, ECS::ComponentHandle<Model> model,
			ECS::ComponentHandle<Transform> transform){
			
		});
	}
	void draw(ECS::World* world, ID3D11DeviceContext* deviceContext,BasicEffect& effect) {
		world->each<Model, Transform>([&](ECS::Entity* ent, ECS::ComponentHandle<Model> model,
			ECS::ComponentHandle<Transform> transform) {
			UINT strides = model->vertexStride;
			UINT offsets = 0;
			for (auto& part : model->modelParts)
			{
				// 设置顶点/索引缓冲区
				deviceContext->IASetVertexBuffers(0, 1, part.vertexBuffer.GetAddressOf(), &strides, &offsets);
				deviceContext->IASetIndexBuffer(part.indexBuffer.Get(), part.indexFormat, 0);

				// 更新数据并应用
				effect.SetWorldMatrix(transform->GetLocalToWorldMatrixXM());
				effect.SetTextureDiffuse(part.texDiffuse.Get());
				effect.SetMaterial(part.material);

				effect.Apply(deviceContext);

				deviceContext->DrawIndexed(part.indexCount, 0, 0);
			}

		});
	}
private:
	//针对着色器文件，主要针对hlsl文件的编译与创建
	void init(ID3D11Device* device){
		// 务必先初始化所有渲染状态，以供下面的特效使用
		// 初始化光栅化器状态
		// 初始化采样器状态
		// 初始化混合状态
		// 初始化深度/模板状态
		RenderStates::InitAll(device);

		//创建顶点着色器
		//创建顶点布局
		//创建像素着色器
		//创建常量缓冲区
		//bool flag = mBasicEffect.InitAll(device);
		//assert(flag == true);
	}
};
#endif
