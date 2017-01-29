#pragma once

class Object
	:public CMeshLoader
{
public:
	Object();
	~Object();

	D3DXHANDLE                  g_hAmbient;
	D3DXHANDLE                  g_hDiffuse;
	D3DXHANDLE                  g_hSpecular;
	D3DXHANDLE                  g_hOpacity;
	D3DXHANDLE                  g_hSpecularPower;
	D3DXHANDLE                  g_hLightColor;
	D3DXHANDLE                  g_hLightPosition;
	D3DXHANDLE                  g_hCameraPosition;
	D3DXHANDLE                  g_hTexture;
	D3DXHANDLE                  g_hTime;
	D3DXHANDLE                  g_hWorld;
	D3DXHANDLE                  g_hWorldViewProjection;

private:
	ID3DXEffect* m_pEffect;

public:
	virtual void CreateEffect(ID3DXEffect* effect);
	virtual void Render(D3DXMATRIX mWorldViewProjection, D3DXMATRIX mWorld, double fTime);
	virtual void RenderSubset(UINT iSubset);
	virtual void DestroyObject();
};

