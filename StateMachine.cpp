#include "StateMachine.h"

StateMachine::StateMachine(LPDIRECT3DDEVICE9 Device)
{
	m_Device = Device;
	m_Init = false;
}

StateMachine::~StateMachine()
{
}
