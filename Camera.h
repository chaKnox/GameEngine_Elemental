#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_
#include "Graphics.h"
#include "GameTimer.h"
class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT, CAR };

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	void Strafe(float units); // left/right
	void Fly(float units);    // up/down
	void Walk(float units);   // forward/backward

	void Pitch(float angle); // rotate on right vector
	void Yaw(float angle);   // rotate on up vector
	void Roll(float angle);  // rotate on look vector

	void GetViewMatrix(D3DXMATRIX* V);
	void SetCameraType(CameraType cameraType) {m_CameraType = cameraType;}
	void GetPosition(D3DXVECTOR3* pos) { *pos = m_Pos; }
	void SetPosition(D3DXVECTOR3* pos) { m_Pos = *pos; }

	void GetRight(D3DXVECTOR3* right) { *right = m_Right; }
	void GetUp(D3DXVECTOR3* up) { *up = m_Up; }
	void GetLook(D3DXVECTOR3* look) { *look = m_Look; }

	void Update();
	D3DXVECTOR3 m_Pos;
private:
	float speed, strafeSpeed, turnSpeed;
	GameTimer m_Timer;
	CameraType  m_CameraType;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Look;
};
#endif // !CAMERA_H_
