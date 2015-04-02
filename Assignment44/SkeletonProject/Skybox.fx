//Skybox shader based off of RBwhitaker

float4x4 World;
float4x4 View;
float4x4 Projection;

float3 CameraPosition;

Texture SkyBoxTexture;
samplerCUBE SkyBoxSampler = sampler_state
{
	texture = <SkyBoxTexture>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};

struct VS_INPUT
{
	float4 Position : POSITION0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float3 TexCoord : TEXCOORD0;
};

VS_OUTPUT VertexShaderFunction(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 worldPosition = mul(input.Position, World);
	float4 viewPosition = mul(worldPosition, View);
	output.Position = mul(viewPosition, Projection);

	float4 VertexPosition = mul(input.Position, World);
	output.TexCoord = VertexPosition - CameraPosition;

	return output;
}

float4 PixelShaderFunction(VS_OUTPUT input) : COLOR0
{
	return texCUBE(SkyBoxSampler, normalize(input.TexCoord));
}

technique SkyBox
{
	pass Pass0
	{
		CULLMODE = CW;
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_2_0 VertexShaderFunction();
		PixelShader = compile ps_2_0 PixelShaderFunction();
	}
};