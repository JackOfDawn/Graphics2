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
		reflectionOn = true;
		blend = .5;
	}
public:
	bool wireFrameOn;
	bool textureOn;
	bool specularOn;
	bool diffuseOn;
	bool phongShader;
	bool reflectionOn;
	void setBlend(float value)
	{
		blend = value;
		if (blend > 1)
			blend = 1;
		if (blend < 0)
			blend = 0;
	}
	float getBlend()
	{
		return blend;
	}
private:
	float blend;
};