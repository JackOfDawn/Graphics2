#pragma once

#include "BaseMaterial.h"

class PhongMaterial : public BaseMaterial
{
public:
	PhongMaterial(IDirect3DDevice9* device);
	virtual ~PhongMaterial(void);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void Update(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& camPos);
	virtual void Render(ID3DXBaseMesh* mesh);

protected:
	D3DXHANDLE m_MatWorldITHandle;
	D3DXHANDLE m_ViewPositionHandle;
};