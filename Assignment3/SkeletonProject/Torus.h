#include "3DClasses\BaseObject3D.h"

class Torus : public BaseObject3D
{
public:
	Torus(float radius1, float radius2, int numSideFacets1,int numSideFacets2);
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* gd3dDevice);

protected:
	virtual void buildDemoCubeVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildDemoCubeIndexBuffer(IDirect3DDevice9* gd3dDevice);

private:
	float radius1;
	float radius2;
	int numSideFacets1;
	int numSideFacets2;

};