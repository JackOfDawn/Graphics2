#include "Sphere.h"
#include "3DClasses\Vertex.h"

#include <assert.h>

Sphere::Sphere(float radius, int numSideFacets) :
radius(radius),
numSideFacets(numSideFacets)
{

}

void Sphere::Create(IDirect3DDevice9* gd3dDevice)
{	
	//HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, 0));
	HR(D3DXCreateSphere(gd3dDevice, radius, numSideFacets / 2, numSideFacets / 2, &m_Mesh, 0));
	HR(m_Mesh->GetVertexBuffer(&m_VertexBuffer));
	HR(m_Mesh->GetIndexBuffer(&m_IndexBuffer));
	m_NumVertices = m_Mesh->GetNumVertices();
	m_NumTriangles = m_Mesh->GetNumFaces();
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
	for (size_t i = 0; i < (unsigned)numSideFacets; i++)
	{
		addTriangle(
			NORTH_INDEX,
			baseVertex + (i % numSideFacets),
			baseVertex + ((i + 1) % numSideFacets));
	}

	
	//draw the meat
	for (size_t j = 0; j < (unsigned)numSideFacets; j++)
	{
		for (size_t i = 0; i < (unsigned)numSideFacets; i++)
		{
			//draw upper right triangle
			
			addTriangle(
				baseVertex + (i % numSideFacets),
				baseVertex + ((i + 1) % numSideFacets) + numSideFacets,

				baseVertex + ((i + 1) % numSideFacets));

			//draw lower left triangle
			addTriangle(
				baseVertex + (i % numSideFacets),
				baseVertex + (i % numSideFacets) + numSideFacets,
				baseVertex + ((i + 1) % numSideFacets) + numSideFacets);


		}
		baseVertex += numSideFacets;
	}
	
	for (size_t i = 0; i < (unsigned)numSideFacets; i++)
	{
		addTriangle(baseVertex + (i %numSideFacets),

			SOUTH_INDEX,
			baseVertex + (i + 1) % numSideFacets);
	}

	HR(m_IndexBuffer->Unlock());
}

void Sphere::Update(float dt)
{
	rotateAroundWorld(0, 1, 0, dt);
	//rotateYawPitchRoll(1, 1, 1, dt);
}