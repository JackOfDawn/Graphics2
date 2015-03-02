#include "Cylinder.h"
#include "3DClasses\Vertex.h"
#include "PhongMaterial.h"

#include <assert.h>

Cylinder::Cylinder(float height, float radius, int sideFacetsNum):
deltaDegrees((PI * 2) / sideFacetsNum),
height(height),
radius(radius),
sideFacetsNum(sideFacetsNum)
{
}

void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{	
	//HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, 0));
	HR(D3DXCreateCylinder(gd3dDevice, radius, radius, height, sideFacetsNum, sideFacetsNum / 2, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));
	m_NumVertices = m_Mesh->GetNumVertices();
	m_NumTriangles = m_Mesh->GetNumFaces();
	m_Material.reset(new PhongMaterial(gd3dDevice));
}

void Cylinder::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICES = sideFacetsNum * 2 + 2; // 2 centerpoints
	m_NumVertices = NUM_VERTICES;

	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	{
		int remainingFacets = sideFacetsNum;
		float remainingDegrees = PI * 2;
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
		v[i] = VertexPos(0, -height / 2, 0);
		++i;

		remainingDegrees = PI * 2;
		remainingFacets = sideFacetsNum;

		while (remainingFacets > 0)
		{
			assert(i < NUM_VERTICES);
			v[i] = VertexPos(cos(remainingDegrees) * radius, height / 2, sin(remainingDegrees) * radius);
			++i;
			remainingDegrees -= deltaDegrees;
			--remainingFacets;
		}

		assert(i < NUM_VERTICES);
		v[i] = VertexPos(0, height / 2, 0);
		++i;

		assert(i == NUM_VERTICES);
	}

	HR(m_VertexBuffer->Unlock());
}

void Cylinder::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_TRIANGLES_PER_CIRCLE = sideFacetsNum;
	const int NUM_SIDE_TRIANGLES = sideFacetsNum * 2;
	const int NUM_TRIANGLES = NUM_TRIANGLES_PER_CIRCLE * 2 + NUM_SIDE_TRIANGLES;
	const int NUM_INDICES = NUM_TRIANGLES * 3;

	const int END_OF_CIRCLE_VERTICES = (m_NumVertices / 2) - 1;
	const int START_SECOND_CIRCLE_VERTICES = END_OF_CIRCLE_VERTICES + 1;
	const int FIRST_CENTERPOINT = (m_NumVertices / 2) - 1;
	const int SECOND_CENTERPOINT = m_NumVertices - 1;

	m_NumTriangles = NUM_TRIANGLES;

	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICES;
	addIndex.numVertices = m_NumVertices;

	addTriangle.addIndex = &addIndex;

	// Draw circle triangles
	for (int i = 0; i < NUM_TRIANGLES_PER_CIRCLE; ++i)
	{
		addTriangle((i + 2) % END_OF_CIRCLE_VERTICES, (i + 1) % END_OF_CIRCLE_VERTICES, FIRST_CENTERPOINT);
		addTriangle(SECOND_CENTERPOINT, (i + START_SECOND_CIRCLE_VERTICES) % SECOND_CENTERPOINT, max((i + 1 + START_SECOND_CIRCLE_VERTICES) % SECOND_CENTERPOINT, START_SECOND_CIRCLE_VERTICES));
	}

	for (int i = 0; i < NUM_SIDE_TRIANGLES / 2; ++i)
	{
		addTriangle((i + START_SECOND_CIRCLE_VERTICES) % m_NumVertices, (i + 1) % END_OF_CIRCLE_VERTICES, max((i + 1 + START_SECOND_CIRCLE_VERTICES) % SECOND_CENTERPOINT, START_SECOND_CIRCLE_VERTICES));
		addTriangle((i + 1) % END_OF_CIRCLE_VERTICES, (i + START_SECOND_CIRCLE_VERTICES) % m_NumVertices, i % END_OF_CIRCLE_VERTICES);
	}

	// Make sure we're not drawing too many triangles
	assert(addIndex.index == NUM_INDICES);
	assert(addTriangle.numTrianglesDrawn == NUM_TRIANGLES);

	HR(m_IndexBuffer->Unlock());
}

void Cylinder::Update(float dt)
{
//	rotateAroundWorld(0, -1, 0, dt);

}