#pragma once

#include "BaseMaterial.h"
#include "RenderOptions.h"

class PhongMaterial : public BaseMaterial
{
public:
	PhongMaterial(IDirect3DDevice9* device);
	virtual ~PhongMaterial(void);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void Update(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat);
	virtual void Render(ID3DXBaseMesh* mesh, RenderOptions options);

protected:
	D3DXHANDLE m_MatWorldITHandle;
	D3DXHANDLE m_ViewPositionHandle;
};