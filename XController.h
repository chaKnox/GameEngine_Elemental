#pragma once
#ifndef XCONTROLLER_H_
#define XCONTROLLER_H_
#include <Windows.h>	
#include <XInput.h>
#include <math.h>

class XController
{
private:
	XINPUT_STATE m_State;

	float m_XDeadZone, m_YDeadZone;
	float m_LeftX, m_LeftY;
	float m_RightX, m_RightY;
	bool m_isConnected;
	int m_ControlID;

public:
	XController();
	~XController();
	int GetPort() { return m_ControlID + 1; }
	bool IsPressed(WORD Button) { return (m_State.Gamepad.wButtons & Button) != 0; }
	XINPUT_GAMEPAD * GetState() { return &m_State.Gamepad; }
	bool CheckConnection();
	void Update();
	void Rumble(int LeftVal, int LightVal);
};

#endif // !XCONTROLLER_H_
