#include <assert.h>
#include "PhongMaterial.h"
#include "Teapot.h"
#include "3DClasses\BaseObject3D.h"

Teapot::Teapot()
{

}

void Teapot::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));
	m_NumVertices = m_Mesh->GetNumVertices();
	m_NumTriangles = m_Mesh->GetNumFaces();
	m_Material.reset(new PhongMaterial(gd3dDevice));
}

void Teapot::Update(float dt)
{

}