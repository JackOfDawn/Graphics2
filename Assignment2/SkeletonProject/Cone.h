#include "3DClasses\BaseObject3D.h"

class Cone : public BaseObject3D
{
public:
	Cone(float height, float radius, int sideFacetsNum);
	virtual void Update(float dt);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);

private:
	float deltaDegrees;
	float height;
	float radius;
	int sideFacetsNum;
};