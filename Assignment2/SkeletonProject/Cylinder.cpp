#include "Cylinder.h"
#include "3DClasses\Vertex.h"

#include <assert.h>

Cylinder::Cylinder(float height, float radius, int sideFacetsNum):
deltaDegrees((PI * 2) / sideFacetsNum),
height(height),
radius(radius),
sideFacetsNum(sideFacetsNum)
{
}

void Cylinder::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICES = sideFacetsNum * 2;
	m_NumVertices = NUM_VERTICES;

	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	{
		float remainingDegrees = PI * 2;
		int i = 0;

		while (remainingDegrees > 0)
		{
			assert(i < NUM_VERTICES);
			v[i] = VertexPos(cos(remainingDegrees) * radius, 0, sin(remainingDegrees) * radius);
			++i;
			remainingDegrees -= deltaDegrees;
		}

		remainingDegrees = PI * 2;

		while (remainingDegrees > 0)
		{
			assert(i < NUM_VERTICES);
			v[i] = VertexPos(cos(remainingDegrees) * radius, height, sin(remainingDegrees) * radius);
			++i;
			remainingDegrees -= deltaDegrees;
		}

		assert(i == NUM_VERTICES);
	}

	HR(m_VertexBuffer->Unlock());
}

void Cylinder::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_TRIANGLES_PER_CIRCLE = (sideFacetsNum - 2);
	const int NUM_SIDE_TRIANGLES = sideFacetsNum * 2;
	const int NUM_TRIANGLES = NUM_TRIANGLES_PER_CIRCLE * 2 + NUM_SIDE_TRIANGLES;
	const int NUM_INDICES = NUM_TRIANGLES * 3;
	m_NumTriangles = NUM_TRIANGLES;

	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICES;
	addIndex.numVertices = m_NumVertices;

	addTriangle.addIndex = &addIndex;

	// Draw circle triangles
	{
		for (int i = 0; i < NUM_TRIANGLES_PER_CIRCLE; ++i)
		{
			addTriangle((i + 2) % (m_NumVertices / 2), (i + 1) % (m_NumVertices / 2), 0);
			addTriangle(sideFacetsNum, (((i + 1) % m_NumVertices) + sideFacetsNum) % m_NumVertices, (((i + 2) % m_NumVertices) + sideFacetsNum) % m_NumVertices);
		}
	}

	{
		for (int i = 0; i < NUM_SIDE_TRIANGLES / 2; ++i)
		{
			addTriangle((i + m_NumVertices / 2) % m_NumVertices, (i + 1) % (m_NumVertices / 2), max((i + 1 + m_NumVertices / 2) % m_NumVertices, m_NumVertices / 2));
			addTriangle((i + 1) % (m_NumVertices / 2), (i + m_NumVertices / 2) % m_NumVertices, i % (m_NumVertices / 2));
		}
	}

	// Make sure we're not drawing too many triangles
	assert(addIndex.index == NUM_INDICES);
	assert(addTriangle.numTrianglesDrawn == NUM_TRIANGLES);

	HR(m_IndexBuffer->Unlock());
}