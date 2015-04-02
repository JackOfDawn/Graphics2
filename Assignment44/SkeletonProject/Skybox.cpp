#include "Skybox.h"
#include "3DClasses\Vertex.h"

SkyBox::SkyBox(float width, float height, float depth) :
width(width),
height(height),
depth(depth)
{
	envCubeTex = NULL;
}

void SkyBox::Create(IDirect3DDevice9* gd3dDevice)
{
	std::string envMapName = "Islands.dds";
	HR(D3DXCreateBox(gd3dDevice, width, height, depth, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));

	//create cube texture
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, envMapName.c_str(),&envCubeTex))

	
	ID3DXBuffer* errors = 0;
	
}

void SkyBox::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& camPos, RenderOptions options)
{

}

void SkyBox::Update(float dt)
{

}
