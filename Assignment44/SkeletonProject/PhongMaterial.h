#pragma once

#include "BaseMaterial.h"
#include "RenderOptions.h"

class PhongMaterial : public BaseMaterial
{
public:
	PhongMaterial(IDirect3DDevice9* device);
	virtual ~PhongMaterial(void);

	virtual void ConnectToEffect(ID3DXEffect* effect);
    virtual void Update( D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat, D3DXVECTOR3& camPos); 
	virtual void Render(ID3DXBaseMesh* mesh, RenderOptions options);

protected:
	/*
	Required Handlers:
		* fvLightPosition [m_LightPosWHandle]
		* fvEyePosition [m_ViewerPosWHandle]
		* matView [---]
		* matViewProjection [m_ViewProjectionMatHandle]
		* fvAmbient (ambient color) [---]
		* fvSpecular (specular color) [m_SpecularColHandle]
		* fvDiffuse (diffuse color) [m_DiffuseColHandle]
		* ka (ambient constant) [---]
		* kd (diffuse constant) [---]
		* ks (specular constant) [---]
		* kt (texture blend constant) [---]
		* bumpiness [---]
		* reflectiveness [---]
		
		Also:
		* base_Tex [---]
		* bump_Tex [---]
		* skyBox_Tex [---]
	*/


	D3DXHANDLE m_ViewHandle;
	D3DXHANDLE m_AmbientColHandle;
	D3DXHANDLE m_kAmbientHandle;
	D3DXHANDLE m_kDiffuseHandle;
	D3DXHANDLE m_kSpecularHandle;
	D3DXHANDLE m_kTextureHandle;
	D3DXHANDLE m_BumpinessHandle;
	D3DXHANDLE m_Reflectiveness;
	D3DXHANDLE m_SpecPow;
	D3DXHANDLE m_CamHandle;

};