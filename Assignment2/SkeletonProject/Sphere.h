#include "3DClasses\BaseObject3D.h"

class Sphere : public BaseObject3D
{
public:
	Sphere(float radius, int numLat, int numLong);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);

private:
	float deltaDegrees;
	float radius;
	int numLat;
	int numLong;
};