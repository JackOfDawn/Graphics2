#include "PhongMaterial.h"
#include "RenderOptions.h"
#include <d3d9.h>
#include <assert.h>

PhongMaterial::PhongMaterial(IDirect3DDevice9* device)
{
	ID3DXBuffer* errorBuffer = nullptr;
	HRESULT result = D3DXCreateEffectFromFile(device, "phong.fx", NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &m_Effect, &errorBuffer);

	if (errorBuffer)
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (!m_Effect)
		::MessageBox(0, "Effect is super broken, dude.", 0, 0);

	//m_DiffuseColor = D3DXVECTOR3(0.0f, 1.0f, 0.5f);
	//m_SpecularColor = D3DXVECTOR3(1.0f, 0.2f, 0.0f);
	m_DiffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_SpecularColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Shininess = 89.0f;

	// Set up handles
	m_WorldMatHandle = m_Effect->GetParameterByName(NULL, "matWorld");
	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(NULL, "matViewProjection");

	{
		m_LightPosWHandle = m_Effect->GetParameterByName(NULL, "dirLightDir");
		// Give default values
		FLOAT dirLightDir[] = { 0, -1, 0 };
		m_Effect->SetFloatArray(m_LightPosWHandle, dirLightDir, 3);
	}

	{
		D3DXHANDLE hDirLightColor = m_Effect->GetParameterByName(NULL, "dirLightColor");
		D3DXVECTOR4 dirLightColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Effect->SetVector(hDirLightColor, &dirLightColor);
	}

	{
		D3DXHANDLE hMaterialAmbient = m_Effect->GetParameterByName(NULL, "materialAmbient");
		D3DXVECTOR4 materialAmbient(0.9f, 0.9f, 0.9f, 1.0f);
		m_Effect->SetVector(hMaterialAmbient, &materialAmbient);
	}

	{
		m_DiffuseColHandle = m_Effect->GetParameterByName(NULL, "materialDiffuse");
		m_Effect->SetFloatArray(m_DiffuseColHandle, (FLOAT*)m_DiffuseColor, 3);
	}

	{
		m_SpecularColHandle = m_Effect->GetParameterByName(NULL, "materialSpecular");
		m_Effect->SetFloatArray(m_SpecularColHandle, (FLOAT*)m_SpecularColor, 3);
	}

	{
		m_ShininessHandle = m_Effect->GetParameterByName(NULL, "materialPower");
		m_Effect->SetFloat(m_ShininessHandle, m_Shininess);
	}

	{
		IDirect3DTexture9* tex = nullptr;
		D3DXCreateTextureFromFile(device, "Earth.jpg", &tex);
		if (!tex)
		{
			::MessageBox(0, "Couldn't load texture", 0, 0);
		}
		else
		{
			D3DXHANDLE hTexture = m_Effect->GetParameterByName(NULL, "texTexture");
			assert(hTexture != NULL);
			m_Effect->SetTexture(hTexture, tex);
			tex->Release();
		}
	}

	// Vertex shader handles
	m_WorldMatHandle = m_Effect->GetParameterByName(NULL, "matWorld");
	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(NULL, "matViewProjection");
	m_MatWorldITHandle = m_Effect->GetParameterByName(NULL, "matWorldIT");
	m_ViewPositionHandle = m_Effect->GetParameterByName(NULL, "viewPosition");

	// Pixel shader handles
	m_UseDiffuseHandle = m_Effect->GetParameterByName(NULL, "useDiffuse");
	assert(m_UseDiffuseHandle != NULL);
	m_UseSpecularHandle = m_Effect->GetParameterByName(NULL, "useSpecular");
	assert(m_UseSpecularHandle != NULL);
	m_UseTextureHandle = m_Effect->GetParameterByName(NULL, "useTexture");
	assert(m_UseTextureHandle != NULL);
}

PhongMaterial::~PhongMaterial()
{
	// eh.
}

void PhongMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	m_Effect = effect;
}

void PhongMaterial::Update(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& camPos)
{
	m_WorldMat = worldMat;
	m_ViewProjectionMat = worldMat * viewProjMat;

	D3DXMATRIX worldMatIT;
	if (!D3DXMatrixInverse(&worldMatIT, NULL, &m_WorldMat))
	{
		::MessageBox(0, "Could not invert matrix", 0, 0);
		return;
	}
	//D3DXVECTOR3 lightPos(camPos.x, -1, camPos.z);
	D3DXMatrixTranspose(&worldMatIT, &worldMatIT);
	HR(m_Effect->SetMatrix(m_MatWorldITHandle, &worldMatIT));
	
	D3DXVECTOR4 viewPosition(viewProjMat._41, viewProjMat._42, viewProjMat._43, viewProjMat._44);
	HR(m_Effect->SetVector(m_ViewPositionHandle, &viewPosition));
	

	HR(m_Effect->SetFloatArray(m_LightPosWHandle, -camPos, 3));

	HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &m_ViewProjectionMat));
	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_WorldMat));

	HR(m_Effect->CommitChanges());
}

void PhongMaterial::Render(ID3DXBaseMesh* mesh, RenderOptions options)
{
	UINT passes = 0;
	D3DXHANDLE technique = m_Effect->GetTechniqueByName("Default_DirectX_Effect");

	m_Effect->SetBool(m_UseDiffuseHandle, options.diffuseOn);
	m_Effect->SetBool(m_UseSpecularHandle, options.specularOn);
	m_Effect->SetBool(m_UseTextureHandle, options.textureOn);

	m_Effect->SetTechnique(technique);
	m_Effect->Begin(&passes, NULL);
	for (UINT pass = 0; pass < passes; ++pass)
	{
		m_Effect->BeginPass(pass);

		HR(mesh->DrawSubset(0));

		m_Effect->EndPass();
	}
	m_Effect->End();
}