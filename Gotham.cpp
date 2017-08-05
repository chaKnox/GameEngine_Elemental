#include "Gotham.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Camera.h"
#include "Model.h"
#include "Vehicle.h"

void Gotham::Initialize()
{
	if (!m_Init)
	{

	D3DXMATRIX l_ProjMat;
	D3DXMATRIX I;
	D3DXMATRIX l_TerMat;
	D3DXMatrixIdentity(&I);

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

	
	D3DXMatrixTransformation(&l_TerMat, NULL, NULL, NULL, NULL, NULL, &D3DXVECTOR3(0, 1, 0));
	//	turn on lighting
	m_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_Skybox = new SkyBox(m_Device, "front.png", "back.png", "left.png", "right.png", "top.png", "bottom1.png");
	m_Terrain = new Terrain(m_Device, "tertex.png", "cityTerrain.raw", 1025, 1025, 5, 0);
	m_Scene = new Model(m_Device, "Models/cityTexTest.x", 200, l_TerMat);
	//m_purpCar = new Model(m_Device, "Models/redCar.x", 1, l_TerMat);
	//m_Camera = new Camera(m_Camera->LANDOBJECT);
	m_RedCar = new Vehicle(m_Device, "Models/redCar.x", 1, l_TerMat);
	m_RedCar->SetTerrain(m_Terrain);
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
	m_RedCar->Update();
}

void Gotham::Render()
{
	if (m_Device)
	{
		m_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
		m_Device->BeginScene();
		
		//	render skybox
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W, m_RedCar->m_Camera->m_Pos.x, m_RedCar->m_Camera->m_Pos.y, m_RedCar->m_Camera->m_Pos.z);
		m_Device->SetTransform(D3DTS_WORLD, &W);

		m_Skybox->Render();

		//	render terrain
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		m_Terrain->Render(&I, false);
		m_Scene->Render();
		m_RedCar->Render();
		//m_purpCar->Render();
		//m_Camera->Update();

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
	delete m_Skybox;
	delete m_Terrain;
	delete m_Scene;
	delete m_purpCar;
}
