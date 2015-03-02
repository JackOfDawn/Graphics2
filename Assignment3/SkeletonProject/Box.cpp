#include <assert.h>
#include "PhongMaterial.h"
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
	m_Material.reset(new PhongMaterial(gd3dDevice));

	int vertNum = 0;

	// Set up vertices and texture mapping!
	SetUpUV([&vertNum](VertexPos in) {
		D3DXVECTOR2 out;
		out.x = 0.0f;
		out.y = 1.0f;
		switch (vertNum % 4)
		{
		case 0:
			out.x = 1.0f;
			out.y = 0.0f;
			break;
		case 1:
			out.x = 0.0f;
			out.y = 0.0f;
			break;
		case 2:
			out.x = 0.0f;
			out.y = 1.0f;
			break;
		case 3:
			out.x = 1.0f;
			out.y = 1.0f;
			break;
		}
		vertNum++;
		return out;
	});
}

void Box::Update(float dt)
{

}