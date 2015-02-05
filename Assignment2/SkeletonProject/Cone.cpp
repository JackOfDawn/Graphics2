#include <assert.h>

#include "Cone.h"
#include "3DClasses\Vertex.h"

Cone::Cone(float height, float radius, int sideFacetsNum):
deltaDegrees((PI * 2) / sideFacetsNum),
height(height),
radius(radius),
sideFacetsNum(sideFacetsNum)
{
}

void Cone::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICES = sideFacetsNum + 1; // Add 1 for top point
	m_NumVertices = NUM_VERTICES;

	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	// Add stuff to the vertex buffer
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

		assert(i < NUM_VERTICES);
		v[i] = VertexPos(0, height, 0);
	}

	HR(m_VertexBuffer->Unlock());
}

void Cone::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICES = sideFacetsNum + 1;
	const int NUM_BOTTOM_TRIANGLES = sideFacetsNum - 2;
	assert(NUM_BOTTOM_TRIANGLES > 0);
	const int NUM_SIDE_TRIANGLES = sideFacetsNum;
	const int NUM_TRIANGLES = NUM_BOTTOM_TRIANGLES + NUM_SIDE_TRIANGLES;
	const int NUM_INDICES = NUM_TRIANGLES * 3;

	m_NumTriangles = NUM_TRIANGLES;

	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICES;
	addIndex.numVertices = NUM_VERTICES;

	addTriangle.addIndex = &addIndex;

	// Draw bottom triangles
	{
		for (int i = 0; i < NUM_BOTTOM_TRIANGLES; ++i)
		{
			addTriangle((i + 2) % NUM_VERTICES, (i + 1) % NUM_VERTICES, 0);
		}
	}

	// Draw side triangles
	{
		const int TIP_INDEX = NUM_VERTICES - 1;
		for (int i = 0; i < NUM_SIDE_TRIANGLES; ++i)
		{
			addTriangle(i, (i + 1) % (NUM_VERTICES - 1), TIP_INDEX);
		}
	}

	// Make sure we're not drawing too many triangles
	assert(addIndex.index == NUM_INDICES);
	assert(addTriangle.numTrianglesDrawn == NUM_TRIANGLES);

	HR(m_IndexBuffer->Unlock());
}
