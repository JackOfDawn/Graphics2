//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>
#include <cassert>
#include <memory>
#include <functional>
#include "../BaseMaterial.h"
#include "Vertex.h"
#include "../RenderOptions.h"

#include "../d3dUtil.h"
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
//=============================================================================

#define PI D3DX_PI

class BaseObject3D
{
protected:	
    D3DXMATRIX                  m_World;

	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;
	ID3DXMesh*					m_Mesh;
	std::unique_ptr<BaseMaterial> m_Material;

	int m_NumVertices;
	int m_NumTriangles;

	bool m_DrawWithTexture;


protected:
    // Replace the code in the following methods
    virtual void buildDemoCubeVertexBuffer( IDirect3DDevice9* gd3dDevice );
    virtual void buildDemoCubeIndexBuffer( IDirect3DDevice9* gd3dDevice );
	
	struct AddIndex
	{
		WORD* k = 0;
		int index = 0;
		int numIndices = 0;
		int numVertices = 0;
		void operator()(WORD v) {
			assert(index < numIndices);
			assert(v < numVertices);
			k[index] = v;
			++index;
		}
	} addIndex;

	struct AddTriangle
	{
		AddIndex* addIndex;
		int numTrianglesDrawn = 0;
		void operator()(WORD v1, WORD v2, WORD v3) {
			(*addIndex)(v1);
			(*addIndex)(v2);
			(*addIndex)(v3);
			++numTrianglesDrawn;
		}
	} addTriangle;

public:
    BaseObject3D(void);
    ~BaseObject3D(void);
	
	 void scale(float sx, float sy, float sz, float dt = 1);
	 void translateTo(float tx, float ty, float tz, float dt = 1);
	 void rotateYawPitchRoll(float rx, float ry, float rz, float dt = 1);
	 void rotateAroundWorld(float rx, float ry, float rz, float dt = 1);


    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice );
	virtual void Update(float dt);
    virtual void Render( IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, RenderOptions options );
	void ToggleDrawWithTexture() { m_DrawWithTexture = !m_DrawWithTexture; }
	void SetUpUV(std::function<D3DXVECTOR2(VertexPos)> f);
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

