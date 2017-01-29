#include "DXUT.h"
#include "gamebase.h"

Object::Object()
{
}

Object::~Object()
{
	DestroyObject();
}

void Object::CreateEffect(ID3DXEffect* effect)
{
	this->m_pEffect = effect;
	
	g_hAmbient = m_pEffect->GetParameterBySemantic(0, "Ambient");
	g_hDiffuse = m_pEffect->GetParameterBySemantic(0, "Diffuse");
	g_hSpecular = m_pEffect->GetParameterBySemantic(0, "Specular");
	g_hOpacity = m_pEffect->GetParameterBySemantic(0, "Opacity");
	g_hSpecularPower = m_pEffect->GetParameterBySemantic(0, "SpecularPower");
	g_hLightColor = m_pEffect->GetParameterBySemantic(0, "LightColor");
	g_hLightPosition = m_pEffect->GetParameterBySemantic(0, "LightPosition");
	g_hCameraPosition = m_pEffect->GetParameterBySemantic(0, "CameraPosition");
	g_hTexture = m_pEffect->GetParameterBySemantic(0, "Texture");
	g_hTime = m_pEffect->GetParameterBySemantic(0, "Time");
	g_hWorld = m_pEffect->GetParameterBySemantic(0, "World");
	g_hWorldViewProjection = m_pEffect->GetParameterBySemantic(0, "WorldViewProjection");
	
	// Store the correct technique handles for each material
	for (UINT i = 0; i < this->GetNumMaterials(); i++)
	{
		Material* pMaterial = this->GetMaterial(i);

		const char* strTechnique = NULL;

		if (pMaterial->pTexture && pMaterial->bSpecular)
			strTechnique = "TexturedSpecular";
		else if (pMaterial->pTexture && !pMaterial->bSpecular)
			strTechnique = "TexturedNoSpecular";
		else if (!pMaterial->pTexture && pMaterial->bSpecular)
			strTechnique = "Specular";
		else if (!pMaterial->pTexture && !pMaterial->bSpecular)
			strTechnique = "NoSpecular";

		pMaterial->hTechnique = effect->GetTechniqueByName(strTechnique);
	}
}

void Object::Render(D3DXMATRIX mWorldViewProjection, D3DXMATRIX mWorld, double fTime)
{
	// Update the effect's variables. 
	m_pEffect->SetMatrix(g_hWorldViewProjection, &mWorldViewProjection);
	m_pEffect->SetMatrix(g_hWorld, &mWorld);
	m_pEffect->SetFloat(g_hTime, (float)fTime);
	m_pEffect->SetValue(g_hCameraPosition, OBJMANAGER()->GetCamera()->GetEyePt(), sizeof(D3DXVECTOR3));
	
	for (UINT iSubset = 0; iSubset < this->GetNumMaterials(); iSubset++)
	{
		RenderSubset(iSubset);
	}
}

void Object::RenderSubset(UINT iSubset)
{
	UINT iPass, cPasses;

	ID3DXMesh* pMesh = this->GetMesh();
	Material* pMaterial = this->GetMaterial(iSubset);
	
	m_pEffect->SetValue(g_hAmbient, pMaterial->vAmbient, sizeof(D3DXVECTOR3));
	m_pEffect->SetValue(g_hDiffuse, pMaterial->vDiffuse, sizeof(D3DXVECTOR3));
	m_pEffect->SetValue(g_hSpecular, pMaterial->vSpecular, sizeof(D3DXVECTOR3));
	m_pEffect->SetTexture(g_hTexture, pMaterial->pTexture);
	m_pEffect->SetFloat(g_hOpacity, pMaterial->fAlpha);
	m_pEffect->SetInt(g_hSpecularPower, pMaterial->nShininess);
	
	m_pEffect->SetTechnique(pMaterial->hTechnique);
	m_pEffect->Begin(&cPasses, 0);
	
	for (iPass = 0; iPass < cPasses; iPass++)
	{
		m_pEffect->BeginPass(iPass);

		// The effect interface queues up the changes and performs them 
		// with the CommitChanges call. You do not need to call CommitChanges if 
		// you are not setting any parameters between the BeginPass and EndPass.
		// V( g_pEffect->CommitChanges() );

		// Render the mesh with the applied technique
		pMesh->DrawSubset(iSubset);

		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

void Object::DestroyObject()
{
	this->Destroy();
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}

	delete this;
}