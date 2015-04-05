//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "d3dApp.h"
#include "RenderOptions.h"
//=============================================================================
class BaseMaterial
{
protected:
    ID3DXEffect*        m_Effect;               // the shader associate effect file

    //-------- Material Parameters -------
    D3DXMATRIX          m_WorldMat;
    D3DXMATRIX          m_ViewProjectionMat;

    D3DXVECTOR3         m_DiffuseColor;
    D3DXVECTOR3         m_SpecularColor;
    float               m_Shininess;            // specualr power


    //---------- Shader Handles ----------
    // Generic shader handles
    D3DXHANDLE          m_WorldMatHandle;    
    D3DXHANDLE          m_ViewProjectionMatHandle;

    D3DXHANDLE          m_LightPosWHandle;       // Position (spot/point) / Direction (directional)
    D3DXHANDLE          m_ViewerPosWHandle;

    // Material specific shader handles
    D3DXHANDLE          m_DiffuseColHandle;    
    D3DXHANDLE          m_SpecularColHandle;       
    D3DXHANDLE          m_ShininessHandle;   

	D3DXHANDLE			m_UseDiffuseHandle;
	D3DXHANDLE			m_UseSpecularHandle;
	D3DXHANDLE			m_UseTextureHandle;
	D3DXHANDLE			m_UseReflectionHandle;
	D3DXHANDLE			m_RSBlendHandle;


public:
    BaseMaterial(void);
    virtual ~BaseMaterial(void);

    virtual void ConnectToEffect( ID3DXEffect* effect ) = 0;
    virtual void Update( D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMat, D3DXVECTOR3& camPos) = 0; 
	virtual void Render(ID3DXBaseMesh* mesh, RenderOptions options) = 0;
};
//=============================================================================

