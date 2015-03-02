#include <assert.h>
#include "Box.h"
#include "3DClasses\Vertex.h"

Box::Box(float width, float height, float depth) :
width(width),
height(height),
depth(depth)
{
}

void Box::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateBox(gd3dDevice, width, height, depth, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));
	m_NumVertices = m_Mesh->GetNumVertices();
	m_NumTriangles = m_Mesh->GetNumFaces();
}

void Box::Update(float dt)
{

}