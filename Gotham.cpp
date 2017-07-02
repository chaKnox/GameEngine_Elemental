#include "Gotham.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Camera.h"



void Gotham::Initialize()
{
	if (!m_Init)
	{

	D3DXMATRIX l_ProjMat;

	D3DXMatrixPerspectiveFovLH(&l_ProjMat, D3DX_PI / 4, screenWidth / screenHeight, 1.0f, 3000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &l_ProjMat);

	//	directional light
	m_MoonLight = D3D::InitDirectionalLight(&D3DXVECTOR3(0.0f, -1.0f, 0.0f), (D3DXCOLOR*)&D3D::WHITE);
	m_Device->SetLight(0, &m_MoonLight);
	m_Device->LightEnable(0, TRUE);

	//	spot light
	m_SpotLight = D3D::InitSpotLight(&D3DXVECTOR3(0.0f, 100.0f, 10.0f), &D3DXVECTOR3(0.0f, -1.0f, 0.0f), (D3DXCOLOR*)&D3D::WHITE);
	m_Device->SetLight(1, &m_SpotLight);
	m_Device->LightEnable(1, TRUE);

	//	point light
	m_PointLight = D3D::InitPointLight(&D3DXVECTOR3(0.0f, 2.0f, 10.0f), (D3DXCOLOR*)&D3D::WHITE);
	m_Device->SetLight(2, &m_PointLight);
	m_Device->LightEnable(2, TRUE);

	//	turn on lighting
	m_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_Skybox = new SkyBox(m_Device, "front.png", "back.png", "left.png", "right.png", "top.png", "bottom1.png");
	m_Terrain = new Terrain(m_Device, "tertex.png", "heightmap.raw", 1025, 1025, 5, 1);
	m_Camera = new Camera(m_Camera->LANDOBJECT);
	m_Init = true;
	}
}

void Gotham::Enter()
{
	if (!m_Init)
	{
		Initialize();
	}
}

void Gotham::Update()
{
	m_Camera->m_Pos.y = (m_Terrain->GetHeight(m_Camera->m_Pos.x, m_Camera->m_Pos.z)+10);
}

void Gotham::Render()
{
	if (m_Device)
	{
		m_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
		m_Device->BeginScene();
		D3DXMATRIX l_View;
		m_Camera->GetViewMatrix(&l_View);
		m_Device->SetTransform(D3DTS_VIEW, &l_View);

		//	render skybox
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W, m_Camera->m_Pos.x, m_Camera->m_Pos.y, m_Camera->m_Pos.z);
		m_Device->SetTransform(D3DTS_WORLD, &W);

		m_Skybox->Render();

		//	render terrain
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		m_Terrain->Render(&I, true);

		m_Camera->Update();

		m_Device->EndScene();
		m_Device->Present(0, 0, 0, 0);
	}
}

void Gotham::Exit()
{
}

void Gotham::PostMessage(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	return;
}

Gotham::Gotham(LPDIRECT3DDEVICE9 Device) : StateMachine(Device)
{
}


Gotham::~Gotham()
{
}
