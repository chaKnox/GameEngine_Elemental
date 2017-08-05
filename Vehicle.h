#pragma once
#ifndef VEHICLE_H_
#define VEHICLE_H_
#include "Graphics.h"
class Model;
class Camera;
class AI;
class XController;
class Terrain;
class Vehicle
{
	LPDIRECT3DDEVICE9 m_pDevice;
	bool m_Player;
	Model* m_Model;
	AI* m_AI;
	D3DXVECTOR3 m_CameraTarget;
	D3DXVECTOR3 m_VehiclePosition;
	XController* m_Controller;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Look;
	float m_MaxSpeed, m_MaxForce;
	Terrain* m_Terrain;
public:
	Vehicle(LPDIRECT3DDEVICE9 Device,char* ModelFile, bool Player, D3DXMATRIX TransformMatrix);
	~Vehicle();
	void SetTerrain(Terrain* terrain) { m_Terrain = terrain; }
	void Enter();
	void Update();
	void Render();
	void Exit();
	void SendEventMessage();
	void RecvMessage();

	Camera* m_Camera;
};
#endif // !VEHICLE_H_
