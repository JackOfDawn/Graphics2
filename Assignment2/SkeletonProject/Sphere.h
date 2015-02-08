#include "3DClasses\BaseObject3D.h"

class Sphere : public BaseObject3D
{
public:
	Sphere(int radius, int numeSideFacets);
	virtual void Update(float dt);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);

private:
	float radius;
	int numSideFacets;
};