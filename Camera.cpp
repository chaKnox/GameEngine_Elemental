#include "Camera.h"



Camera::Camera()
{
	speed = 50.0f;
	strafeSpeed = 50.0f;
	turnSpeed = 2.0f;
	
	m_CameraType = AIRCRAFT;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::Camera(CameraType cameraType)
{
	speed = 50.0f;
	strafeSpeed = 50.0f;
	turnSpeed = 2.0f;
	
	m_CameraType = cameraType;

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


Camera::~Camera()
{
}

void Camera::Strafe(float units)
{
	// move only on xz plane for land object
	if (m_CameraType == LANDOBJECT)
		m_Pos += D3DXVECTOR3(m_Right.x, 0.0f, m_Right.z) * units;

	if (m_CameraType == AIRCRAFT)
		m_Pos += m_Right * units;
}

void Camera::Fly(float units)
{
	if (m_CameraType == AIRCRAFT)
		m_Pos += m_Up * units;
}

void Camera::Walk(float units)
{
	// move only on xz plane for land object
	if (m_CameraType == LANDOBJECT)
		m_Pos += D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * units;

	if (m_CameraType == AIRCRAFT)
		m_Pos += m_Look * units;
}

void Camera::Pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_Right, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&m_Up, &m_Up, &T);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
}

void Camera::Yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if (m_CameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if (m_CameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &m_Up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
}

void Camera::Roll(float angle)
{
	// only roll for aircraft type
	if (m_CameraType == AIRCRAFT)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_Look, angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
		D3DXVec3TransformCoord(&m_Up, &m_Up, &T);
	}
}

void Camera::GetViewMatrix(D3DXMATRIX * V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&m_Look, &m_Look);

	D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
	D3DXVec3Normalize(&m_Right, &m_Right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&m_Right, &m_Pos);
	float y = -D3DXVec3Dot(&m_Up, &m_Pos);
	float z = -D3DXVec3Dot(&m_Look, &m_Pos);

	(*V)(0, 0) = m_Right.x; (*V)(0, 1) = m_Up.x; (*V)(0, 2) = m_Look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = m_Right.y; (*V)(1, 1) = m_Up.y; (*V)(1, 2) = m_Look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = m_Right.z; (*V)(2, 1) = m_Up.z; (*V)(2, 2) = m_Look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::Update()
{
	m_Timer.Tick();
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) {
		Yaw(m_Timer.DeltaTime()* turnSpeed);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f) {
		Yaw(-m_Timer.DeltaTime() * turnSpeed);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000f) {
		Pitch(-m_Timer.DeltaTime()* turnSpeed);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000f) {
		Pitch(m_Timer.DeltaTime() * turnSpeed);
	}
	if (GetAsyncKeyState('W') & 0x8000f) {
		Walk(m_Timer.DeltaTime() * speed);
	}
	if (GetAsyncKeyState('S') & 0x8000f) {
		Walk(-m_Timer.DeltaTime() * speed);
	}
	if (GetAsyncKeyState('A') & 0x8000f) {
		Strafe(-m_Timer.DeltaTime() * strafeSpeed);
	}
	if (GetAsyncKeyState('D') & 0x8000f) {
		Strafe(m_Timer.DeltaTime() * strafeSpeed);
	}
}
