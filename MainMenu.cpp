#include "MainMenu.h"



MainMenu::MainMenu( LPDIRECT3DDEVICE9 Device)
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
	if (m_Background)
		delete m_Background;
	if (m_ButtonDefault)
		delete m_ButtonDefault;
	if (m_ButtonOver)
		delete m_ButtonOver;
	if (m_ButtonOver)
		delete m_ButtonOver;
	if (wc)
		delete wc;
}

bool MainMenu::Initialize()
{
	if (!m_Init)
	{
		m_Background = new Texture(m_Device, "test.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1});
		m_ButtonDefault = new Texture(m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonOver = new Texture(m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		wc = new WindowControl(NULL, NULL);
		wc->SetSprite(m_Sprite);
		wc->SetTexture(m_Background);
		
		ButtonControl* temp;
		temp = new ButtonControl(wc->GetThis(), 1, D3DXVECTOR2{ 100,100 }, m_Device);
		wc->AddChildControl(temp);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Testing");
		m_Init = true;
	}
	return m_Init;
}

void MainMenu::OnRender()
{
	wc->OnRender();
}

void MainMenu::Update(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	//wc->PostMessage(WM_PAINT, wParam, lParam, NULL);
	wc->PostMessage(msg, wParam, lParam, NULL);
}

