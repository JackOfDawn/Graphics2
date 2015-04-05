#include "PhongMaterial.h"
#include "RenderOptions.h"
#include <d3d9.h>
#include <assert.h>

PhongMaterial::PhongMaterial(IDirect3DDevice9* device)
{
	ID3DXBuffer* errorBuffer = nullptr;
	HRESULT result = D3DXCreateEffectFromFile(device, "ReflectionPhong.fx", NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &m_Effect, &errorBuffer);

	if (errorBuffer)
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (!m_Effect)
		::MessageBox(0, "Effect is super broken, dude.", 0, 0);

	m_DiffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_SpecularColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Shininess = 89.0f;

	// Set up handles

	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(NULL, "matViewProjection");
	m_ViewHandle = m_Effect->GetParameterByName(NULL, "matView");
	m_WorldMatHandle = m_Effect->GetParameterByName(NULL, "World");




	{
		m_LightPosWHandle = m_Effect->GetParameterByName(NULL, "fvLightPosition");
		FLOAT lightPosition[] = { -100, 0, -100 };
		m_Effect->SetFloatArray(m_LightPosWHandle, lightPosition, 3);
	}

	{
		m_ViewerPosWHandle = m_Effect->GetParameterByName(NULL, "fvEyePosition");
		FLOAT eyePosition[] = { 100, 100, 100 };
		m_Effect->SetFloatArray(m_ViewerPosWHandle, eyePosition, 3);
	}

	{
		m_AmbientColHandle = m_Effect->GetParameterByName(NULL, "fvAmbient");
		FLOAT ambientCol[] = { 1.0f, 1.0f, 1.0f };
		m_Effect->SetFloatArray(m_AmbientColHandle, ambientCol, 3);
	}

	{
		m_SpecularColHandle = m_Effect->GetParameterByName(NULL, "fvSpecular");
		m_Effect->SetFloatArray(m_SpecularColHandle, m_SpecularColor, 3);
	}

	{
		m_DiffuseColHandle = m_Effect->GetParameterByName(NULL, "fvDiffuse");
		m_Effect->SetFloatArray(m_DiffuseColHandle, m_DiffuseColor, 3);
	}

	{
		m_kAmbientHandle = m_Effect->GetParameterByName(NULL, "ka");
		FLOAT ambientConst = .10f;
		m_Effect->SetFloat(m_kAmbientHandle, ambientConst);
	}

	{
		m_kDiffuseHandle = m_Effect->GetParameterByName(NULL, "kd");
		FLOAT diffuseConst = .30f;
		m_Effect->SetFloat(m_kDiffuseHandle, diffuseConst);
	}

	{
		m_kSpecularHandle = m_Effect->GetParameterByName(NULL, "ks");
		FLOAT specularConst = 1.0f;
		m_Effect->SetFloat(m_kSpecularHandle, specularConst);
	}

	{
		m_kTextureHandle = m_Effect->GetParameterByName(NULL, "kt");
		FLOAT textureConst = 1.0f;
		m_Effect->SetFloat(m_kTextureHandle, textureConst);
	}

	{
		m_CamHandle = m_Effect->GetParameterByName(NULL, "CameraPosition");
		
		m_Effect->SetFloatArray(m_CamHandle, m_DiffuseColor, 3);
	}

	{
		m_BumpinessHandle = m_Effect->GetParameterByName(NULL, "bumpiness");
		FLOAT bumpConst = 1.0f;
		m_Effect->SetFloat(m_BumpinessHandle, bumpConst);
	}

	{
		m_Reflectiveness = m_Effect->GetParameterByName(NULL, "reflectiveness");
		FLOAT reflectiveConst = 1.0f;
		m_Effect->SetFloat(m_Reflectiveness, reflectiveConst);
	}

	{
		m_SpecPow = m_Effect->GetParameterByName(NULL, "fSpecularPower");
		FLOAT specPow = 1.0f;
		m_Effect->SetFloat(m_SpecPow, specPow);
	}
	{
		//set texture
		IDirect3DTexture9* tex = nullptr;
		D3DXCreateTextureFromFile(device, "Fieldstone.tga", &tex);
		if (!tex)
		{
			::MessageBox(0, "Couldn't load texture", 0, 0);
		}
		else
		{
			D3DXHANDLE hTexture = m_Effect->GetParameterByName(NULL, "base_Tex");
			assert(hTexture != NULL);
			m_Effect->SetTexture(hTexture, tex);
			tex->Release();
		}
		//bump
		tex = nullptr;
		D3DXCreateTextureFromFile(device, "FieldstoneBumpDOT3.tga", &tex);
		if (!tex)
		{
			::MessageBox(0, "Couldn't load texture", 0, 0);
		}
		else
		{
			D3DXHANDLE hTexture = m_Effect->GetParameterByName(NULL, "bump_Tex");
			assert(hTexture != NULL);
			m_Effect->SetTexture(hTexture, tex);
			tex->Release();
		}

		//set env texture
		IDirect3DCubeTexture9* cubeTex = nullptr;
		D3DXCreateCubeTextureFromFile(device, "SkyBox.dds", &cubeTex);
		if (!cubeTex)
		{
			::MessageBox(0, "Couldn't load CUBE", 0, 0);
		}
		else
		{
			D3DXHANDLE hCubeTexture = m_Effect->GetParameterByName(NULL, "skyBox_Tex");
			assert(hCubeTexture != NULL);
			m_Effect->SetTexture(hCubeTexture, cubeTex);
			cubeTex->Release();
		}

	}

}
PhongMaterial::~PhongMaterial()
{
	// eh.
}

void PhongMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	m_Effect = effect;
}

void PhongMaterial::Update(D3DXMATRIX& worldMat, D3DXMATRIX& viewMat, D3DXMATRIX& projMatrix, D3DXVECTOR3& camPos)
{
	m_WorldMat = worldMat;
	m_ViewProjectionMat = worldMat * viewMat * projMatrix;

	//FLOAT arr[] = { viewMat._41, viewMat._42, viewMat._43 };
	//HR(m_Effect->SetFloatArray(m_ViewerPosWHandle, arr, 3));
	HR(m_Effect->SetFloatArray(m_CamHandle, camPos, 3));
	HR(m_Effect->SetMatrix(m_WorldMatHandle, &worldMat));
	HR(m_Effect->SetMatrix(m_ViewHandle, &viewMat));

	HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &m_ViewProjectionMat));

	HR(m_Effect->CommitChanges());
}

void PhongMaterial::Render(ID3DXBaseMesh* mesh, RenderOptions options)
{
	UINT passes = 0;
	D3DXHANDLE technique = m_Effect->GetTechniqueByName("Default_DirectX_Effect");

	m_Effect->SetFloat(m_kTextureHandle, options.textureOn ? 1 : 0);
	m_Effect->SetFloat(m_Reflectiveness, options.reflectionOn ? options.getBlend() : 0);
	m_Effect->SetFloat(m_BumpinessHandle, options.normalMappingOn ? options.getStrength() : 0);
	m_Effect->SetFloat(m_SpecPow, options.specPow);

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