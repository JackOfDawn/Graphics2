#include <assert.h>

#include "PhongMaterial.h"
#include "Cone.h"
#include "3DClasses\Vertex.h"

Cone::Cone(float height, float radius, int sideFacetsNum):
deltaDegrees((PI * 2) / sideFacetsNum),
height(height),
radius(radius),
sideFacetsNum(sideFacetsNum)
{
}


void Cone::Create(IDirect3DDevice9* gd3dDevice)
{	
	
	//HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, 0));
	HR(D3DXCreateCylinder(gd3dDevice, radius, 0, height, sideFacetsNum / 2, sideFacetsNum / 2, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));
	m_NumVertices = m_Mesh->GetNumVertices();
	m_NumTriangles = m_Mesh->GetNumFaces();

	m_Material.reset(new PhongMaterial(gd3dDevice));

	SetUpUV([this](VertexPos in) {
		D3DXVECTOR2 out;
		out.x = in.pos.x / radius;
		out.y = in.pos.y / height;
		return out;
	});
}

void Cone::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{

	const int NUM_VERTICES = sideFacetsNum + 1 + 1; // Add 1 for top point, one for the center
	m_NumVertices = NUM_VERTICES;
	
	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	// Add stuff to the vertex buffer
	{
		float remainingDegrees = PI * 2;
		int remainingFacets = sideFacetsNum;
		int i = 0;

		while (remainingFacets > 0)
		{
			assert(i < NUM_VERTICES);
			v[i] = VertexPos(cos(remainingDegrees) * radius, -height / 2, sin(remainingDegrees) * radius);
			++i;
			remainingDegrees -= deltaDegrees;
			--remainingFacets;
		}

		assert(i < NUM_VERTICES);
		v[i] = VertexPos(0, height / 2, 0);
		++i;

		assert(i < NUM_VERTICES);
		v[i] = VertexPos(0, -height / 2, 0);
	}

	HR(m_VertexBuffer->Unlock());
}

void Cone::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICES = m_NumVertices;
	const int NUM_BOTTOM_TRIANGLES = sideFacetsNum;
	const int NUM_SIDE_TRIANGLES = sideFacetsNum;
	const int NUM_TRIANGLES = NUM_BOTTOM_TRIANGLES + NUM_SIDE_TRIANGLES;
	const int NUM_INDICES = NUM_TRIANGLES * 3;
	const int END_OF_BOTTOM_VERTICES = NUM_VERTICES - 2;
	const int TIP_INDEX = NUM_VERTICES - 2;

	m_NumTriangles = NUM_TRIANGLES;

	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICES;
	addIndex.numVertices = NUM_VERTICES;

	addTriangle.addIndex = &addIndex;

	// Draw bottom triangles
	for (int i = 0; i < NUM_BOTTOM_TRIANGLES; ++i)
	{
		const int CENTER_INDEX = NUM_VERTICES - 1;
		addTriangle((i + 2) % END_OF_BOTTOM_VERTICES, (i + 1) % END_OF_BOTTOM_VERTICES, CENTER_INDEX);
	}

	// Draw side triangles
	for (int i = 0; i < NUM_SIDE_TRIANGLES; ++i)
	{
		addTriangle((i + 1) % END_OF_BOTTOM_VERTICES, (i + 2) % END_OF_BOTTOM_VERTICES, TIP_INDEX);
	}

	// Make sure we're not drawing too many triangles
	assert(addIndex.index == NUM_INDICES);
	assert(addTriangle.numTrianglesDrawn == NUM_TRIANGLES);

	HR(m_IndexBuffer->Unlock());
}

void Cone::Update(float dt)
{
//	rotateAroundWorld(0, -1, 0, dt);
}