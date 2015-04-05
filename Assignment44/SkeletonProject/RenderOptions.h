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
		normalMappingOn = true;
		blend = .5;
		strength = .5f;
		specPow = 2.0f;
	}
public:
	bool wireFrameOn;
	bool textureOn;
	bool specularOn;
	bool diffuseOn;
	bool phongShader;
	bool reflectionOn;
	bool normalMappingOn;

	float specPow;

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
	void setStrength(float value)
	{
		strength = value;
		if (strength > 1)
			strength = 1;
		if (strength < 0)
			strength = 0;
	}
	float getStrength()
	{
		return strength;
	}
private:
	float blend;
	float strength;
};