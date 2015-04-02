#pragma once
#include "3DClasses\BaseObject3D.h"

class SkyBox : public	BaseObject3D
{
public:
	SkyBox(float width, float height, float depth);
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* );
	virtual void Render( IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& camPos, RenderOptions options );

private:
	float width;
	float height;
	float depth;

	IDirect3DCubeTexture9* envCubeTex;
};