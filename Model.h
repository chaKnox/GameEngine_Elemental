#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "Graphics.h"
#include <vector>
#include <string>
class Model
{
protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	ID3DXMesh* m_Mesh;
	std::vector<LPDIRECT3DTEXTURE9> m_Textures;
	std::vector<D3DMATERIAL9> m_Materials;
	D3DXMATRIX m_TransMat;
	float m_Scale;
	std::string m_File;
	std::vector<D3D::BoundingBox*> m_BoundVol;
public:
	Model(LPDIRECT3DDEVICE9 Device,char* file, float scale, D3DXMATRIX worldTrans);
	~Model();
	void LoadMeshes();
	void Render();
	void ReleaseMeshes();
	void UpdateLocation(D3DXVECTOR3 NewLocation);
	void UpdateTransMat(D3DXMATRIX TransMat) { m_TransMat *= TransMat; }
};

#endif