#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "UIControls.h"
#include <vector>
#include "UIWrappers.h"	

class MainMenu :public WindowControl
{
private:
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
public:
	MainMenu(UIBase* parent, int vecPos, LPDIRECT3DDEVICE9 Device);
	~MainMenu();
	bool Initialize();
};

#endif // !MAINMENU_H
