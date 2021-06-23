#include "Basic.hlsli"

float4 PS(BillboardVertex pIn) : SV_Target
{
	float4 texColor = g_TexArray.Sample(g_Sam, float3(pIn.Tex, pIn.PrimID % 4));
	clip(texColor.a - 0.05f);

	pIn.NormalW = normalize(pIn.NormalW);

	float3 toEyeW = normalize(g_EyePosW - pIn.PosW);
	float distToEye = distance(g_EyePosW, pIn.PosW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (int i = 0; i < 5; ++i)
	{
		ComputeDirectionalLight(g_Material, g_DirLight[i], pIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 litColor = texColor * (ambient + diffuse) + spec;

	litColor.a = texColor.a * g_Material.Diffuse.a;
	litColor = litColor * pIn.Color;
	return litColor;
}