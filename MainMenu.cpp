#include "MainMenu.h"



MainMenu::MainMenu(UIBase* parent, int vecPos, LPDIRECT3DDEVICE9 Device):WindowControl(parent,vecPos)
{
	m_Init = false;
	m_Device = Device;
	m_Sprite = NULL;
	m_Sprite = new Sprite(m_Device);
}

MainMenu::~MainMenu()
{
	if (m_Sprite)
		delete m_Sprite;
}

bool MainMenu::Initialize()
{
	if (!m_Init)
	{
		
		LoadCanvasFromFile("grey_background.png");
		ButtonControl* l_TempButton;
		l_TempButton = new ButtonControl(GetThis(), 1, m_Device);
		l_TempButton->SetTextures("button.png", "buttonOver.png");
	}
	return false;
}

