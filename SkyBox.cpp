#include "SkyBox.h"



SkyBox::SkyBox(LPDIRECT3DDEVICE9 Device, char* front, char* back, char* left, char* right, char* top, char* bottom)
{
	m_Device = Device;
	float size = 100;
	D3D::Vertex_UV l_SkyMesh[24] =
	{
		//Front
		{ -size,-size,size, 0,1 },
		{ -size,size,size, 0,0 },
		{ size,-size,size, 1,1 },
		{ size,size,size, 1,0 },
		//Back
		{ size, -size, -size, 0,1 },
		{ size, size, -size, 0,0 },
		{ -size,-size, -size, 1,1 },
		{ -size,size, -size, 1,0 },
		//Left
		{ -size, -size, -size, 0,1 },
		{ -size, size, -size, 0,0 },
		{ -size,-size, size, 1,1 },
		{ -size, size, size, 1,0 },
		//Right
		{ size,-size,size, 0,1 },
		{ size,size,size, 0,0 },
		{ size,-size,-size, 1,1 },
		{ size,size,-size, 1,0 },
		//Top
		{ -size,size,size, 0,1 },
		{ -size,size,-size, 0,0 },
		{ size,size,size, 1,1 },
		{ size,size,-size, 1,0 },
		//Bottom
		{ -size,-size,-size, 0,1 },
		{ -size,-size,size, 0,0 },
		{ size,-size,-size, 1,1 },
		{ size,-size,size, 1,0 }
	};
	if (FAILED(m_Device->CreateVertexBuffer(sizeof(D3D::Vertex_UV) * 24, 0, D3D::FVF, D3DPOOL_MANAGED, &m_VertexBuffer, 0)))
		MessageBox(NULL, "Skybox VB Create Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, front, &m_Texture[0])))
		MessageBox(NULL, "Skybox Texture Front Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, back, &m_Texture[1])))
		MessageBox(NULL, "Skybox Texture Back Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, left, &m_Texture[2])))
		MessageBox(NULL, "Skybox Texture Left Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, right, &m_Texture[3])))
		MessageBox(NULL, "Skybox Texture Right Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, top, &m_Texture[4])))
		MessageBox(NULL, "Skybox Texture Top Failed", "Error", MB_OK);
	if (FAILED(D3DXCreateTextureFromFile(m_Device, bottom, &m_Texture[5])))
		MessageBox(NULL, "Skybox Texture Bottom Failed", "Error", MB_OK);
}


SkyBox::~SkyBox()
{
}

void SkyBox::Render()
{
	m_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_Device->SetRenderState(D3DRS_ZENABLE, false);
	m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_Device->SetFVF(D3D::FVF);
	m_Device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(D3D::Vertex_UV));
	for (int i = 0; i < 6; i++)
	{
		m_Device->SetTexture(0, m_Texture[i]);
		m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
	m_Device->SetRenderState(D3DRS_ZENABLE, true);
	m_Device->SetRenderState(D3DRS_LIGHTING, true);
}
