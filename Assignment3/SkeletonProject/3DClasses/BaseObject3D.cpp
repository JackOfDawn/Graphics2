//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#include "BaseObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
#include "../PhongMaterial.h"
#include "../GouraudMaterial.h"
//=============================================================================
BaseObject3D::BaseObject3D(void)
{
    m_VertexBuffer = NULL;
    m_IndexBuffer = NULL;
	m_Mesh = NULL;

    D3DXMatrixIdentity(&m_World);
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
    ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Create( IDirect3DDevice9* gd3dDevice )
{
    buildDemoCubeVertexBuffer( gd3dDevice );
    buildDemoCubeIndexBuffer( gd3dDevice );
}

//-----------------------------------------------------------------------------
void BaseObject3D::Render( IDirect3DDevice9* gd3dDevice,
    D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& camPos, RenderOptions options )
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_NumVertices);
    GfxStats::GetInstance()->addTriangles(m_NumTriangles);

    // Set the buffers and format
    HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
	HR(gd3dDevice->SetIndices(m_IndexBuffer));
	HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

    // Set matrices and model relevant render date
	//HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
	//HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	//HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));	
    
    // Send to render
	if (m_Mesh)
	{
		if (m_PhongMaterial != nullptr && m_GouraudMaterial != nullptr)
		{
			D3DXMATRIX viewProjMat = view * projection;
			if (options.phongShader)
			{
				m_PhongMaterial->Update(m_World, viewProjMat, camPos);
				m_PhongMaterial->Render(m_Mesh, options);
			}
			else
			{
				m_GouraudMaterial->Update(m_World, viewProjMat, camPos);
				m_GouraudMaterial->Render(m_Mesh, options);
			}
		}
		//else
			//HR(m_Mesh->DrawSubset(0));
	}
	else
	{
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_NumVertices, 0, m_NumTriangles));
	}
}

//-----------------------------------------------------------------------------
void BaseObject3D::buildDemoCubeVertexBuffer( IDirect3DDevice9* gd3dDevice )
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(8 * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's vertex data.

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexPos(-1.0f, -1.0f, -1.0f);
	v[1] = VertexPos(-1.0f,  1.0f, -1.0f);
	v[2] = VertexPos( 1.0f,  1.0f, -1.0f);
	v[3] = VertexPos( 1.0f, -1.0f, -1.0f);
	v[4] = VertexPos(-1.0f, -1.0f,  1.0f);
	v[5] = VertexPos(-1.0f,  1.0f,  1.0f);
	v[6] = VertexPos( 1.0f,  1.0f,  1.0f);
	v[7] = VertexPos( 1.0f, -1.0f,  1.0f);

	HR(m_VertexBuffer->Unlock());
}

//-----------------------------------------------------------------------------
void BaseObject3D::buildDemoCubeIndexBuffer( IDirect3DDevice9* gd3dDevice )
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's index data.

	WORD* k = 0;

	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	// Front face.
	k[0] = 0; k[1] = 1; k[2] = 2;
	k[3] = 0; k[4] = 2; k[5] = 3;

	// Back face.
	k[6] = 4; k[7]  = 6; k[8]  = 5;
	k[9] = 4; k[10] = 7; k[11] = 6;

	// Left face.
	k[12] = 4; k[13] = 5; k[14] = 1;
	k[15] = 4; k[16] = 1; k[17] = 0;

	// Right face.
	k[18] = 3; k[19] = 2; k[20] = 6;
	k[21] = 3; k[22] = 6; k[23] = 7;

	// Top face.
	k[24] = 1; k[25] = 5; k[26] = 6;
	k[27] = 1; k[28] = 6; k[29] = 2;

	// Bottom face.
	k[30] = 4; k[31] = 0; k[32] = 3;
	k[33] = 4; k[34] = 3; k[35] = 7;

	HR(m_IndexBuffer->Unlock());
}


//----------------------------------------------------------------------------
void BaseObject3D::Update(float dt)
{
	//D3DXMATRIX transform;
	//D3DXMatrixIdentity(&transform);
	//D3DXMatrixRotationYawPitchRoll(&transform, 3, 2, 1);
	//D3DXMatrixMultiply(&m_World, &m_World, &transform);
}


//----------------------------------------------------------------------------
void BaseObject3D::scale(float sx, float sy, float sz, float dt)
{
	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);
	D3DXMatrixScaling(&transform, sx, sy, sz);
	D3DXMatrixMultiply(&m_World, &m_World, &transform);
}


//----------------------------------------------------------------------------
void BaseObject3D::translateTo(float tx, float ty, float tz, float dt)
{
	m_World._41 = tx;
	m_World._42 = ty;
	m_World._43 = tz;
}


//----------------------------------------------------------------------------
void BaseObject3D::rotateAroundWorld(float rx, float ry, float rz, float dt)
{
	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);
	D3DXMatrixRotationYawPitchRoll(&transform, ry * dt, rx * dt, rz * dt);
	D3DXMatrixMultiply(&m_World, &m_World, &transform);
	//D3DXMatrixRotationYawPitchRoll(&m_World, ry * dt, rx * dt, rz * dt);
}

//----------------------------------------------------------------------------

void BaseObject3D::rotateYawPitchRoll(float rx, float ry, float rz, float dt)
{
	D3DXVECTOR3 oldCoord(m_World._41, m_World._42, m_World._43);
	translateTo(0, 0, 0);

	rotateAroundWorld(rx, ry, rz, dt);
	translateTo(oldCoord.x, oldCoord.y, oldCoord.z, dt);
}


//=============================================================================

void BaseObject3D::SetUpUV(std::function<D3DXVECTOR2(VertexPos)> f)
{

		D3DVERTEXELEMENT9 elem[MAXD3DDECLLENGTH];
		UINT numElements;
		VertexPos::Decl->GetDeclaration(elem, &numElements);
		ID3DXMesh* mesh;
		m_Mesh->CloneMesh(0, elem, gd3dDevice, &mesh);
		VertexPos* verts;
		HR(mesh->LockVertexBuffer(0, (void**)&verts));

		for (unsigned i = 0; i < mesh->GetNumVertices(); ++i)
		{
			verts[i].texcoord = f(verts[i]);
		}

		HR(mesh->UnlockVertexBuffer());

		m_Mesh->Release();
		m_Mesh = mesh;
}