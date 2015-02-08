#include "3DClasses\Vertex.h"
#include "Torus.h"
#include <assert.h>

Torus::Torus(float radius1, float radius2, int numSideFacets1, int numSideFacets2) :
radius1(radius1),
radius2(radius2),
numSideFacets1(numSideFacets1),
numSideFacets2(numSideFacets2)
{

}

void Torus::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_RINGS = numSideFacets1;
	const int NUM_SIDES = numSideFacets1;
	const int NUM_VERTS = NUM_RINGS * NUM_SIDES;
	m_NumVertices = NUM_VERTS;
	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTS * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	//Lets build key donut points
	{
		//large circle
		float deltaPhi = (( 2 * D3DX_PI) / NUM_RINGS); //used for the main circle
		float phi = 0;
		
		//smallCircle
		D3DXVECTOR3 sideCenter;
		float deltaTheta = (2 * D3DX_PI) / NUM_SIDES;
		float theta = 0;
		D3DXVECTOR3 sideValue;
		int k = 0;

		for (int i = 0; i < NUM_RINGS; ++i)
		{
			//get the center point of the new ring
			phi += deltaPhi;
			sideCenter.x = cos(phi) * radius1;
			sideCenter.y = sin(phi) * radius1;
			sideCenter.z = 0;

			for (int j = 0; j < NUM_SIDES; j++)
			{
				//Q = P + r2 * cos(theta) * W + r2 * sin(theta) * Z
				theta += deltaTheta;

				assert(k < NUM_VERTS);
				v[k] = VertexPos(
					sideCenter.x + (sideCenter.x * (radius2 * cos(theta))), // P.x + rs*cos(theta)*W.x
					sideCenter.y + (sideCenter.y * (radius2 * cos(theta))), // P.y + rs*cos(theta)*W.y
					sideCenter.z + (radius2 * sin(theta)));
				k++;
				
			}
		}
	}

	HR(m_VertexBuffer->Unlock());

}

void Torus::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTS = m_NumVertices + 100;
	const int NUM_RINGS = numSideFacets1;
	const int NUM_SIDES = numSideFacets2;
	const int NEXT_RING = NUM_SIDES;
	const int NUM_TRIS = 2 * (NUM_RINGS * NUM_SIDES);
	const int NUM_INDICIES = 3 * (NUM_TRIS);
	m_NumTriangles = NUM_TRIS;

	HR(gd3dDevice->CreateIndexBuffer(NUM_INDICIES * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	HR(m_IndexBuffer->Lock(0, 0, (void**)&addIndex.k, 0));
	addIndex.numIndices = NUM_INDICIES;
	addIndex.numVertices = NUM_VERTS;

	addTriangle.addIndex = &addIndex;
	int currentRingIndex = 0;
	
	for (size_t r = 0; r < NUM_RINGS; r++)
	{
		for (size_t s = 0; s < NUM_SIDES; s++)
		{
			//draw upperright tri
			addTriangle(
				currentRingIndex + (s  % NUM_SIDES),
				(currentRingIndex + (s % NUM_SIDES) + NEXT_RING) % (NUM_RINGS * NUM_SIDES),
				(currentRingIndex + ((s + 1) % NUM_SIDES) + NEXT_RING) % (NUM_RINGS * NUM_SIDES));
			//draw lowerright tri
			addTriangle(
				currentRingIndex + (s % NUM_SIDES),
				(currentRingIndex + (s + 1) % NUM_SIDES + NEXT_RING) % (NUM_RINGS * NUM_SIDES),
				currentRingIndex + (s + 1) % NUM_SIDES);
		}
		currentRingIndex += NUM_SIDES;
	}

}

void Torus::Update(float dt)
{
	//scale(2, 2, 2, 1);
	rotateAroundWorld(0, -1, 0, dt);
	rotateYawPitchRoll(0, 2, 0, dt);
//		D3DXMATRIX transform;
//	D3DXMatrixIdentity(&transform);
//	D3DXMatrixRotationYawPitchRoll(&transform, 3 * dt, 2 * dt, 1 * dt);
//	D3DXMatrixMultiply(&m_World, &m_World, &transform);
}

