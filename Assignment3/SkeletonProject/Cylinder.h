#include "3DClasses\BaseObject3D.h"

class Cylinder : public BaseObject3D
{
public:
	Cylinder(float height, float radius, int sideFacetsNum);
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* gd3dDevice);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);
	
private:
	float deltaDegrees;
	float height;
	float radius;
	int sideFacetsNum;
};