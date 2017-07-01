#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include "Graphics.h"
class SkyBox
{
	LPDIRECT3DDEVICE9 m_Device;
	//D3D::Vertex_UV m_SkyMesh[24];
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	LPDIRECT3DTEXTURE9	m_Texture[6];
public:
	SkyBox(LPDIRECT3DDEVICE9 Device,char* front, char* back, char* left, char* right, char* top, char* bottom);
	~SkyBox();
	void Render();
};

#endif
