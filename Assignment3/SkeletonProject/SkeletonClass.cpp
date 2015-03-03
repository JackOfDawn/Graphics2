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
#include "Box.h"
#include "Teapot.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Torus.h"
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
	oldKeyO = false;
	oldKeyT = false;
	oldKeyW = false;
	oldKeyD = false;
	oldKeyS = false;
	oldKeyR = false;
	cameraPos = D3DXVECTOR3(0, 0, 0);

	renderOptions = RenderOptions();
	
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}
	InitAllVertexDeclarations();
	HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

	mCameraRadius    = 50.0f;
	mCameraRotationY = 0;
	mCameraRotationX = 0;
	//mCameraHeight    = 5.0f;
	mUp.x = 0;
	mUp.y = 1;
	mUp.z = 0;

	float yOffset = -5;
	//Create the planet
	int counter = 0;
	
	m_Objects.push_back(new Sphere(10, 50));
    m_Objects[counter]->Create( gd3dDevice );
	//m_Objects[counter]->translateTo(0, 0 + yOffset, 0);
	//m_Objects[counter]->rotateYawPitchRoll(0, 0, D3DX_PI / 2);
	//m_Objects[counter]->scale(10, 10, 10);
	counter++;
	
	m_Objects.push_back(new Box(10, 10, 10));
	m_Objects[counter]->Create(gd3dDevice);
	//m_Objects[counter]->translateTo(0, -13 + yOffset, 0);
	//m_Objects[counter]->rotateYawPitchRoll(D3DX_PI / 2, D3DX_PI / 2, D3DX_PI / 4);
	counter++;


	//CreatetWoRings
	m_Objects.push_back(new Teapot());
	m_Objects[counter]->Create(gd3dDevice);
	m_Objects[counter]->scale(5, 5, 5);
	//m_Objects[counter]->translateTo(0, 0 + yOffset, 15);
	//m_Objects[counter]->rotateYawPitchRoll(D3DX_PI/2, D3DX_PI / 2, D3DX_PI/4);
	counter++;

	//create Cylinder Core
	m_Objects.push_back(new Cylinder(10, 10, 50));
	m_Objects[counter]->Create(gd3dDevice);
	//m_Objects[counter]->translateTo(0, 12 + yOffset, 0);
	//m_Objects[counter]->rotateYawPitchRoll(D3DX_PI / 2, D3DX_PI / 2, D3DX_PI / 2);

	counter++;
	//create mini cones
	m_Objects.push_back(new Cone(10, 5, 30));
	m_Objects[counter]->Create(gd3dDevice);
	//m_Objects[counter]->translateTo(0, 0 + yOffset, -15);
	counter++;	
	
	onResetDevice();

	//other ring
	m_Objects.push_back(new Torus(2, 10, 50, 40));
	m_Objects[counter]->Create(gd3dDevice);
	//m_Objects[counter]->translateTo(0, 0 + yOffset, -15);
	//m_Objects[counter]->rotateYawPitchRoll(D3DX_PI/2, D3DX_PI / 2, D3DX_PI/4);
	counter++;

	m_CurrentObjectIter = m_Objects.begin();
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
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	//Check for vertex shader version 2.0 suppport
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
		return false;
	//Check pixel shader support;
	if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
		return false;

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
	//gDInput->mouseDZ();
		mCameraRadius += gDInput->mouseDZ() / 50.0f;

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

	bool newKeyO = gDInput->keyDown(DIKEYBOARD_O);
	bool newKeyT = gDInput->keyDown(DIKEYBOARD_T);
	bool newKeyW = gDInput->keyDown(DIKEYBOARD_W);
	bool newKeyS = gDInput->keyDown(DIKEYBOARD_S);
	bool newKeyD = gDInput->keyDown(DIKEYBOARD_D);
	if (!oldKeyO && newKeyO)
	{
		if (++m_CurrentObjectIter == m_Objects.end())
		{
			m_CurrentObjectIter = m_Objects.begin();
		}
	}
	if (!oldKeyT && newKeyT)
	{

		renderOptions.textureOn = !renderOptions.textureOn;
	}
	if (!oldKeyW && newKeyW)
	{
		//WireFrame
		renderOptions.wireFrameOn = !renderOptions.wireFrameOn;
	}
	if (!oldKeyS && newKeyS)
	{
		// Turn off specular (is this possible?)
		renderOptions.specularOn = !renderOptions.specularOn;
	}
	if (!oldKeyD && newKeyD)
	{
		// Turn off diffuse (is this possible?)
		renderOptions.diffuseOn = !renderOptions.diffuseOn;
	}

	oldKeyO = newKeyO;
	oldKeyT = newKeyT;
	oldKeyW = newKeyW;
	oldKeyS = newKeyS;
	oldKeyD = newKeyD;



	for each (BaseObject3D* shape in m_Objects)
	{
		shape->Update(dt);
	}
	

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
	if (!renderOptions.wireFrameOn)
	{
		HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	}
	else
	{
		HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
	}
    // Render all the objects
	/*
	for (m_CurrentObjectIter = m_Objects.begin(); m_CurrentObjectIter != m_Objects.end(); ++m_CurrentObjectIter)
	{
		(*m_CurrentObjectIter)->Render(gd3dDevice, mView, mProj);
	}
    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
    {
        m_Objects[obj]->Render( gd3dDevice, mView, mProj );
    }
	*/
	(*m_CurrentObjectIter)->Render(gd3dDevice, mView, mProj, cameraPos, renderOptions);

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
	cameraPos = D3DXVECTOR3(x, y, z);

	if (mCameraRotationX > D3DX_PI / 2 && mCameraRotationX < (3 * D3DX_PI) / 2)
		mUp.y = -1;
	else
		mUp.y = 1;
	//D3DXVECTOR3 pos2(mCameraHeight, )
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);

	
	D3DXMatrixLookAtLH(&mView, &cameraPos, &target, &mUp);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}