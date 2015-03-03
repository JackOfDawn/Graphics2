#pragma once

struct RenderOptions
{
	RenderOptions()
	{
		wireFrameOn = false;
		textureOn	= true;
		specularOn	= true;
		diffuseOn	= true;
		phongShader = true;
	}
public:
	bool wireFrameOn;
	bool textureOn;
	bool specularOn;
	bool diffuseOn;
	bool phongShader;
};