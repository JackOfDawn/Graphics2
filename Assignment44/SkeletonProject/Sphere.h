#include "3DClasses\BaseObject3D.h"
#include "BaseMaterial.h"
#include <memory>

class Sphere : public BaseObject3D
{
public:
	Sphere(float radius, int numeSideFacets);
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* gd3dDevice);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);

private:
	float radius;
	int numSideFacets;
};