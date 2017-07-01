#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <d3d9.h>

class StateMachine
{
public:
	StateMachine(LPDIRECT3DDEVICE9 Device);
	~StateMachine();
	virtual void Initialize()=0;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void PostMessage(UINT msg, WPARAM wParam, LPARAM lParam, void* Data) = 0;
protected:
	bool m_Joystick;
	bool m_Init;
	LPDIRECT3DDEVICE9 m_Device;
};
#endif // !STATEMACHINE_H
