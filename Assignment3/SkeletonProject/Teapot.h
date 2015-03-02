#include "3DClasses\BaseObject3D.h"

class Teapot : public BaseObject3D
{
public:
	Teapot();
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* gd3dDevice);


};