#pragma once
#ifndef GOTHAM_H
#define GOTHAM_H
#include "StateMachine.h"
//#include "Camera.h"
class Camera;
class SkyBox;
class Terrain;
class Model;
class Vehicle;
class Gotham : public StateMachine
{
private:
	//Camera* m_Camera;
	D3DLIGHT9 m_MoonLight;
	D3DLIGHT9 m_SpotLight;
	D3DLIGHT9 m_PointLight;
protected:
	SkyBox* m_Skybox;
	Terrain* m_Terrain;
	Model* m_Scene;
	Model* m_purpCar;
	Vehicle* m_RedCar;
public:
	virtual void Initialize();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void PostMessage(UINT msg, WPARAM wParam, LPARAM lParam, void* Data);

	Gotham(LPDIRECT3DDEVICE9 Device);
	~Gotham();
};

#endif