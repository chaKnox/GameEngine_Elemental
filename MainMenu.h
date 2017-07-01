#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "StateMachine.h"
#include "UIWrappers.h"
#include "UIControls.h"

class MainMenu : public StateMachine
{
private:
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	WindowControl* wc;
public:
	MainMenu(LPDIRECT3DDEVICE9 Device);
	~MainMenu();
	void Initialize();
	void Enter();
	void Render();
	void Update();
	void Exit();
	void PostMessage(UINT msg, WPARAM wParam, LPARAM lParam, void* Data);
};

#endif // !MAINMENU_H
