#include "Sphere.h"
#include "3DClasses\Vertex.h"

#include <assert.h>

Sphere::Sphere(float radius, int numLat, int numLong) :
radius(radius),
numLat(numLat),
numLong(numLong)
{

}

void Sphere::buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	const int NUM_VERTICIES = numLong * numLat + 2; //Add 2 for the top and the bottom
	m_NumVertices = NUM_VERTICIES;

	HR(gd3dDevice->CreateVertexBuffer(NUM_VERTICIES * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	//Add to the vertex buffer
	{
		float deltaPhi = (2 * D3DX_PI) / numLong;
		float deltaTheta = D3DX_PI / (numLat + 2);
		float theta = 0;
		float phi = 0;
		int i = 0;

		//add the norh end
		assert(i < NUM_VERTICIES);
		v[i] = VertexPos(0, 0, radius);
		++i;

		//add the meat of the sphere
		for (int t = 0; t < numLat; t++)
		{
			theta += deltaTheta;
			for (int p = 0; p < numLong; p++)
			{
				phi += deltaPhi;
				assert(i < NUM_VERTICIES);
				v[i] = VertexPos(radius * sin(theta) * cos(phi), radius * sin(theta) * sin(phi), radius * cos(theta));
				++i;
			}
		}
		
		//cap off the sandwich
		assert(i < NUM_VERTICIES);
		v[i] = VertexPos(0, 0, -radius);
		++i;

		HR(m_VertexBuffer->Unlock());
	}
}

void Sphere::buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice)
{

}