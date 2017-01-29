#pragma once

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	static ObjectManager* Instance() { return &singleton; }

private:
	static ObjectManager singleton;

	CModelViewerCamera* pCamera;

	ID3DXFont* pFont;
	ID3DXEffect* pEffect;

private:
	Object* pCup;

public:
	void LoadFile(IDirect3DDevice9* pd3dDevice);
	void AddObject();

public:
	void OnCreate(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	void OnRender(IDirect3DDevice9* pd3dDevice, double fTime);
	void OnLost();
	void OnDestory();
	void CameraMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CModelViewerCamera* GetCamera() { return pCamera; }
};

#define OBJMANAGER() ObjectManager::Instance()