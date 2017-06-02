#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "UIControls.h"
#include <vector>
#include "UIWrappers.h"	
#include "Input.h"
class MainMenu
{
private:
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Input* m_Input;
	Mouse* m_Mouse;
	WindowControl* wc;
	std::vector<ButtonControl*> bc;
public:
	MainMenu( LPDIRECT3DDEVICE9 Device);
	~MainMenu();
	bool Initialize();
	void OnRender();
	void Update(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
};

#endif // !MAINMENU_H
