#include "Sphere.h"
#include "3DClasses\Vertex.h"

#include <assert.h>

Sphere::Sphere(int radius, int numSideFacets) :
radius(radius),
numSideFacets(numSideFacets)
{

}

void Sphere::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICIES = numSideFacets * numSideFacets + 2 + numSideFacets; //Add 2 for the top and the bottom
	m_NumVertices = NUM_VERTICIES;

	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICIES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	//Add to the vertex buffer
	{
		float deltaPhi = (2 * D3DX_PI) / numSideFacets;
		float deltaTheta = D3DX_PI / (numSideFacets + 2);
		float theta = 0;
		float phi = 0;
		int i = 0;

		//add the norh end
		assert(i < NUM_VERTICIES);
		v[i] = VertexPos(0, 0, radius);
		++i;

		//add the meat of the spher
		for (int t = 0; t <= numSideFacets; t++)
		{
			theta += deltaTheta;
			for (int p = 0; p < numSideFacets; p++)
			{
				phi += deltaPhi;
				assert(i < NUM_VERTICIES);
				v[i] = VertexPos(radius * sin(theta) * cos(phi), radius * sin(theta) * sin(phi), radius * cos(theta));
				++i;
			}
		}
		
		//cap off the sandwich
		assert(i < NUM_VERTICIES);
		v[i] = VertexPos(0,0, -radius);
		++i;

		HR(m_VertexBuffer->Unlock());
	}
}

void Sphere::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICIES = m_NumVertices;
	const int NORTH_INDEX = 0;
	const int SOUTH_INDEX = NUM_VERTICIES - 1;
	const int NUM_TRIANGLES = (numSideFacets  * numSideFacets) * 2 + numSideFacets *2;
	//const int NUM_TRIANGLES = NUM_QUADS * 2 + 2 * (numLong);
	const int NUM_INDICES = NUM_TRIANGLES * 4;


	m_NumTriangles = NUM_TRIANGLES;
	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICES;
	addIndex.numVertices = NUM_VERTICIES;

	addTriangle.addIndex = &addIndex;

	//draw the north triangles
	int baseVertex = 1;
	for (size_t i = 0; i < numSideFacets; i++)
	{
		addTriangle(NORTH_INDEX, baseVertex + ((i + 1) % numSideFacets), baseVertex + (i % numSideFacets));
	}

	
	//draw the meat
	for (size_t j = 0; j < numSideFacets; j++)
	{
		for (size_t i = 0; i < numSideFacets; i++)
		{
			//draw upper right triangle
			
			addTriangle(
				baseVertex + (i % numSideFacets),
				baseVertex + ((i + 1) % numSideFacets),
				baseVertex + ((i + 1) % numSideFacets) + numSideFacets);

			//draw lower left triangle
			addTriangle(
				baseVertex + (i % numSideFacets),
				baseVertex + ((i + 1) % numSideFacets) + numSideFacets,
				baseVertex + (i % numSideFacets) + numSideFacets);


		}
		baseVertex += numSideFacets;
	}
	
	for (size_t i = 0; i < numSideFacets; i++)
	{
		addTriangle(baseVertex + (i %numSideFacets), baseVertex + (i + 1) % numSideFacets, SOUTH_INDEX);
	}





























	/*
	//draw north Triangles
	for (int i = 0; i <= numLong; i++)
	{
		if (i != numLong)
			addTriangle(NORTH_INDEX, (i + 1), i);
		else
			addTriangle(NORTH_INDEX, (i + 1) % numLong, i);
	}

	//draw the meat
	int baseIndex = 1;
	for (int i = 0; i < numLat - 1; i++)
	{
		for (size_t j = 0; j < numLong; j++)
		{
			int topLeftIndex = baseIndex + j;
			//build topright triangle
			if (j != numLong - 1)
				addTriangle(topLeftIndex, topLeftIndex + 1, topLeftIndex + 1 + numLong);
			
			//build bottom left triangle
			if (j != numLong - 1)
				addTriangle(topLeftIndex, topLeftIndex + numLong + 1,  topLeftIndex + numLong);
		}
		baseIndex += numLong;
	}
	*/
	// draw the south
	
	//for (size_t i = 0; i < numLong; i++)
	//{
	//	if (i != numLong - 1)
	//		addTriangle(baseIndex + i, baseIndex + i + 1, SOUTH_INDEX);
	//	else
	//		addTriangle(baseIndex + i, baseIndex, SOUTH_INDEX);
	//}

	
	HR(m_IndexBuffer->Unlock());
}