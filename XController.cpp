#include "XController.h"



XController::XController()
{
	m_XDeadZone = 0.25f;
	m_YDeadZone = 0.25f;
	m_LeftX = 0;
	m_LeftY = 0;
	m_RightX = 0;
	m_RightY = 0;
	m_isConnected = false;
	m_ControlID = -1;
}


XController::~XController()
{
}

bool XController::CheckConnection()
{
	int l_ControllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && (l_ControllerId == -1); i++) 
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			l_ControllerId = i;
	}

	m_ControlID = l_ControllerId;


	return l_ControllerId != -1;
}

void XController::Update()
{
	if (m_ControlID == -1)
		CheckConnection();

	if (m_ControlID != -1)
	{
		ZeroMemory(&m_State, sizeof(XINPUT_STATE));
		if (XInputGetState(m_ControlID, &m_State) != ERROR_SUCCESS)
		{
			m_ControlID = -1;
		}

		//check for input and make deadzones
		float l_NormLX = fmaxf(-1, (float)m_State.Gamepad.sThumbLX / 32767);
		float l_NormLY = fmaxf(-1, (float)m_State.Gamepad.sThumbLY / 32767);

		m_LeftX = (fabs(l_NormLX) < m_XDeadZone ? 0 : (fabs(l_NormLX) - m_XDeadZone) * (l_NormLX / fabs(l_NormLX)));
		m_LeftY = (fabs(l_NormLY) < m_YDeadZone ? 0 : (fabs(l_NormLY) - m_YDeadZone) * (l_NormLY / fabs(l_NormLY)));

		if (m_XDeadZone > 0) m_LeftX *= 1 / (1 - m_XDeadZone);
		if (m_YDeadZone > 0) m_LeftY *= 1 / (1 - m_YDeadZone);

		float l_NormRX = fmaxf(-1, (float)m_State.Gamepad.sThumbRX / 32767);
		float l_NormRY = fmaxf(-1, (float)m_State.Gamepad.sThumbRY / 32767);

		m_RightX = (fabs(l_NormRX) < m_XDeadZone ? 0 : (fabs(l_NormRX) - m_XDeadZone) * (l_NormRX / fabs(l_NormRX)));
		m_RightY = (fabs(l_NormRY) < m_YDeadZone ? 0 : (fabs(l_NormRY) - m_YDeadZone) * (l_NormRY / fabs(l_NormRY)));

		if (m_XDeadZone > 0) m_RightX *= 1 / (1 - m_XDeadZone);
		if (m_YDeadZone > 0) m_RightY *= 1 / (1 - m_YDeadZone);
	}
}

void XController::Rumble(int LeftVal, int RightVal)
{
	// create vibration state
	XINPUT_VIBRATION l_Vibration;

	// clear vibration struct
	ZeroMemory(&l_Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	l_Vibration.wLeftMotorSpeed = LeftVal;
	l_Vibration.wRightMotorSpeed = RightVal;

	// Vibrate the controller
	XInputSetState(m_ControlID, &l_Vibration);
}
