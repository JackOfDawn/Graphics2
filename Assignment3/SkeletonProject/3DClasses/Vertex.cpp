//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#include "Vertex.h"
#include "../d3dUtil.h"
//=============================================================================
// Initialize static variables.
IDirect3DVertexDeclaration9* VertexPos::Decl = 0;
//=============================================================================
void InitAllVertexDeclarations()
{
	// VertexPos

	D3DVERTEXELEMENT9 VertexPosElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, sizeof(D3DXVECTOR3) * 1, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{0, sizeof(D3DXVECTOR3) * 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexPosElements, &VertexPos::Decl));
}
//-----------------------------------------------------------------------------
void DestroyAllVertexDeclarations()
{
	ReleaseCOM(VertexPos::Decl);
}
//=============================================================================