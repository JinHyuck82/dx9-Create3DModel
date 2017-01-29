#include "DXUT.h"
#include "gamebase.h"

ObjectManager ObjectManager::singleton;

ObjectManager::ObjectManager()
{
	pCamera = new CModelViewerCamera;
}

ObjectManager::~ObjectManager()
{
	OnDestory();
}

void ObjectManager::OnCreate(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	D3DXCreateFont(pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Arial", &pFont);

	LoadFile(pd3dDevice);

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	WCHAR str[MAX_PATH];

	// Read the D3DX effect file
	DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"MeshFromOBJ.fx");

	// If this fails, there should be debug output as to 
	// they the .fx file failed to compile
	D3DXCreateEffectFromFile(pd3dDevice, str, NULL, NULL, dwShaderFlags,
		NULL, &pEffect, NULL);
		
	pCup->CreateEffect(pEffect);

	D3DXVECTOR3 vecEye(2.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vecAt(0.0f, 0.0f, -0.0f);
	pCamera->SetViewParams(&vecEye, &vecAt);

	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	pCamera->SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	pCamera->SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
}

void ObjectManager::LoadFile(IDirect3DDevice9* pd3dDevice)
{
	AddObject();

	pCup->Create(pd3dDevice, L"media\\cup.obj");
}

void ObjectManager::AddObject()
{
	pCup = new Object;
}

void ObjectManager::CameraMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	pCamera->HandleMessages(hWnd, uMsg, wParam, lParam);
}

void ObjectManager::OnRender(IDirect3DDevice9* pd3dDevice, double fTime)
{
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;

	mWorld = *pCamera->GetWorldMatrix();
	mView = *pCamera->GetViewMatrix();
	mProj = *pCamera->GetProjMatrix();

	mWorldViewProjection = mWorld * mView * mProj;

	pCup->Render(mWorldViewProjection, mWorld, fTime);
}

void ObjectManager::OnLost()
{
	if (pFont)
	{
		pFont->OnLostDevice();
	}
	if (pEffect)
	{
		pEffect->OnLostDevice();
	}
}

void ObjectManager::OnDestory()
{
	if (pEffect)
	{
		pEffect->Release();
		pEffect = NULL;
	}
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;
	}

	pCup->DestroyObject();
}