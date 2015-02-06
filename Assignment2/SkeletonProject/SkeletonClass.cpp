//=============================================================================
//                          SkeletonClass.cpp
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>

#include "SkeletonClass.h"
#include "3DClasses\BaseObject3D.h"
#include "3DClasses\Vertex.h"
#include "Cone.h"
#include "Cylinder.h"
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	SkeletonClass app(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	return gd3dApp->run();
}

SkeletonClass::SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	mCameraRadius    = 10.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraRotationX = 1.2 * D3DX_PI;
	mCameraHeight    = 5.0f;
	mUp.x = 0;
	mUp.y = 1;
	mUp.z = 0;

    // repleace or add to the following object creation
    //m_Objects.push_back( new BaseObject3D );
	//m_Objects.push_back(new Cone(1, 1, 50));
	m_Objects.push_back(new Cylinder(1, 1, 50));

    m_Objects[0]->Create( gd3dDevice );

	onResetDevice();

	InitAllVertexDeclarations();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        delete m_Objects[obj];
    m_Objects.clear();

	DestroyAllVertexDeclarations();
}

bool SkeletonClass::checkDeviceCaps()
{
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void SkeletonClass::updateScene(float dt)
{
	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	// Get snapshot of input devices.
	gDInput->poll();

	// Check input.
	if (gDInput->keyDown(DIK_W))
		mUp.y = 1.0f;
	if( gDInput->keyDown(DIK_S) )	 
		mUp.y = -1;

	// Divide by 50 to make mouse less sensitive. 
	mCameraRotationY += gDInput->mouseDX() / 100.0f;
	mCameraRotationX += gDInput->mouseDY() / 100.0f;
	// += gDInput->mouseDZ() / 25.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if( fabsf(mCameraRotationY) >= 2.0f * D3DX_PI ) 
		mCameraRotationY = 0.0f;
	// If we rotate over 360 degrees, just roll back to 0
	if (fabsf(mCameraRotationX) >= 2.0f * D3DX_PI)
		mCameraRotationX = 0.0f;
	else if (mCameraRotationX < 0.0f)
		mCameraRotationX = 1.999f * D3DX_PI;


	// Don't let radius get too small.
	if( mCameraRadius < 5.0f )
		mCameraRadius = 5.0f;

	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();
}


void SkeletonClass::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

    // Set render statws for the entire scene here:
//	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));

    // Render all the objects
    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
    {
        m_Objects[obj]->Render( gd3dDevice, mView, mProj );
    }

    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	float x = mCameraRadius * sinf(mCameraRotationY) * cosf(mCameraRotationX);
	float y = mCameraRadius * sinf(mCameraRotationX);
	float z = mCameraRadius * cosf(mCameraRotationY) * cosf(mCameraRotationX); 
	D3DXVECTOR3 pos(x, y, z);

	if (mCameraRotationX > D3DX_PI / 2 && mCameraRotationX < (3 * D3DX_PI) / 2)
		mUp.y = -1;
	else
		mUp.y = 1;
	//D3DXVECTOR3 pos2(mCameraHeight, )
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);

	
	D3DXMatrixLookAtLH(&mView, &pos, &target, &mUp);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}