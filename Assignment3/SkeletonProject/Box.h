#include "3DClasses\BaseObject3D.h"

class Box : public BaseObject3D
{
public:
	Box(float width, float height, float depth);
	virtual void Update(float dt);
	virtual void Create(IDirect3DDevice9* gd3dDevice);
private:
	float width;
	float height;
	float depth;
};