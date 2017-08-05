#include "Vehicle.h"
#include "Camera.h"
#include "XController.h"
#include "Model.h"
#include "AI.h"
#include "Terrain.h"
#include "GameTimer.h"
extern GameTimer g_TickTock;

Vehicle::Vehicle(LPDIRECT3DDEVICE9 Device, char* ModelFile, bool Player, D3DXMATRIX TransformMatrix)
{
	m_pDevice = Device;
	m_Model = new Model(m_pDevice, ModelFile, 1.0f, TransformMatrix);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_MaxForce = 50.0f;
	m_MaxSpeed = 100.0f;
	if (Player)
	{
		m_Player = Player;
		m_Camera = new Camera(Camera::CAR);
	}
}


Vehicle::~Vehicle()
{
	delete m_Camera;
	delete m_Model;
}


void Vehicle::Enter()
{
}

void Vehicle::Update()
{
	D3DXMATRIX T;

	if (GetAsyncKeyState('W') & 0x8000f) 
	{
		m_VehiclePosition += D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * (m_MaxSpeed*g_TickTock.DeltaTime());
	}
	if (GetAsyncKeyState('S') & 0x8000f) 
	{
		m_VehiclePosition += (D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * (m_MaxSpeed*g_TickTock.DeltaTime())*-1);
	}
	if (GetAsyncKeyState('A') & 0x8000f) 
	{
		D3DXMatrixRotationY(&T, (-m_MaxForce*g_TickTock.DeltaTime()));
		// rotate _right and _look around _up or y-axis
		D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
		D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
	}
	if (GetAsyncKeyState('D') & 0x8000f) 
	{
		D3DXMatrixRotationY(&T, (m_MaxForce*g_TickTock.DeltaTime()));
		// rotate _right and _look around _up or y-axis
		D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
		D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
	}
	m_Camera->Update();
	m_VehiclePosition.y = (m_Terrain->GetHeight(m_VehiclePosition.x, m_VehiclePosition.z) + 10);
	m_Model->UpdateLocation(m_VehiclePosition);
	m_Model->UpdateTransMat(T);
	m_Camera->SetTarget(m_VehiclePosition);
}

void Vehicle::Render()
{
	D3DXMATRIX l_View;
	m_Camera->GetViewMatrix(&l_View);
	m_pDevice->SetTransform(D3DTS_VIEW, &l_View);
	m_Model->Render();
}

void Vehicle::Exit()
{
}

void Vehicle::SendEventMessage()
{
}

void Vehicle::RecvMessage()
{
}
