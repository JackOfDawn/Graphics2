#include "Skybox.h"
#include "3DClasses\Vertex.h"

SkyBox::SkyBox(float width, float height, float depth) :
width(width),
height(height),
depth(depth)
{
	envCubeTex = NULL;
	m_Effect = NULL;
}

void SkyBox::Create(IDirect3DDevice9* gd3dDevice)
{
	std::string envMapName = "Islands.dds";
	HR(D3DXCreateBox(gd3dDevice, width, height, depth, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));

	//create cube texture
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, envMapName.c_str(), &envCubeTex));

	
	ID3DXBuffer* errors = 0;
	HRESULT result = D3DXCreateEffectFromFile(gd3dDevice, "Skybox.fx", NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &m_Effect, &errors);
	if (errors)
	{
		::MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		errors->Release();
	}
	//set up handles
	m_WorldMatHandle = m_Effect->GetParameterByName(NULL, "World");
	m_ViewMatHandle = m_Effect->GetParameterByName(NULL, "View");
	m_ProjectionHandle = m_Effect->GetParameterByName(NULL, "Projection");
	m_CameraPosHandle = m_Effect->GetParameterByName(NULL, "CameraPosition");

	//texture
	if (!envCubeTex)
	{
		::MessageBox(0, "COULD NOT FINT THE TEXTURE", 0, 0);
	}
	else
	{
		D3DXHANDLE htexCUBE = m_Effect->GetParameterByName(NULL, "SkyBoxTexture");
		m_Effect->SetTexture(htexCUBE, envCubeTex);
	}

}

void SkyBox::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& camPos, RenderOptions options)
{
	//Update the shader
	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_World));
	HR(m_Effect->SetMatrix(m_ViewMatHandle, &view));
	HR(m_Effect->SetMatrix(m_ProjectionHandle, &projection));

	HR(m_Effect->SetFloatArray(m_CameraPosHandle, camPos, 3));

	UINT passes = 0;
	D3DXHANDLE technique = m_Effect->GetTechniqueByName("SkyBox");

	m_Effect->SetTechnique(technique);
	m_Effect->Begin(&passes, NULL);
	for (UINT pass = 0; pass < passes; ++pass)
	{
		m_Effect->BeginPass(pass);
		HR(m_Mesh->DrawSubset(0));
		m_Effect->EndPass();
	}
	m_Effect->End();
}

void SkyBox::Update(float dt)
{

}
