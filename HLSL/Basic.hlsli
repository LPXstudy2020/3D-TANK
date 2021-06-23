#include "LightHelper.hlsli"

Texture2D g_DiffuseMap : register(t0);
TextureCube g_TexCube : register(t1);
Texture2DArray g_TexArray:register(t2);

SamplerState g_Sam : register(s0);


cbuffer CBChangesEveryInstanceDrawing : register(b0)
{
	matrix g_World;
	matrix g_WorldInvTranspose;
}

cbuffer CBChangesEveryObjectDrawing : register(b1)
{
	Material g_Material;
}

cbuffer CBDrawingStates : register(b2)
{
	int g_TextureUsed;
	int g_ReflectionEnabled;
	float2 g_Pad;
}

cbuffer CBChangesEveryFrame : register(b3)
{
	matrix g_View;
	float3 g_EyePosW;
	float g_Pad2;
}

cbuffer CBChangesOnResize : register(b4)
{
	matrix g_Proj;
}

cbuffer CBChangesRarely : register(b5)
{
	DirectionalLight g_DirLight[5];
	PointLight g_PointLight[5];
	SpotLight g_SpotLight[5];
}

cbuffer CBChangesParticle : register(b6)
{
	float lifespan;
	float speed;
	float time;
	//float position;
}

struct VertexPosNormalTex
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
};

struct InstancePosNormalTex
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
	matrix World : World;
	matrix WorldInvTranspose : WorldInvTranspose;
};

//point 
struct PointSprite 
{
	float3 PosW:POSITION;
	float2 SizeW:SIZE;
	float4 Color:COLOR;
};

struct BillboardVertex
{
	float4 PosH : SV_POSITION;
	float4 Color: COLOR;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	uint PrimID : SV_PrimitiveID;
};