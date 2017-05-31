#include "UIControls.h"
#include "Graphics.h"

WindowControl::WindowControl(UIBase * parent, int vecPos) : UIBase(parent, vecPos)
{
	if (parent == NULL)
		m_IsParentWindow = true;
	m_IsMouseDown = false;
	m_MouseClickPos={ 0,0 };
}

WindowControl::~WindowControl()
{
	if (GetTexture())
		delete GetTexture();

}

bool WindowControl::OnRender(WPARAM Key, LPARAM Extended)
{
	D3DXVECTOR2 l_ControlAbsPos;
	l_ControlAbsPos.x = 0;
	l_ControlAbsPos.y = 0;
	GetAbsolutePosition(&l_ControlAbsPos);
	GetTexture()->SetTranslation(l_ControlAbsPos);
	GetSprite()->DrawTexture(GetTexture());
	GetTexture()->SetTranslation(D3DXVECTOR2{ 0,0 });
	return true;
}

void WindowControl::OnMouseDown(int Button, int x, int y)
{
	if (!m_IsParentWindow)
	{
		m_MouseClickPos={float(x),float(y)};
		D3DXVECTOR2 Abs{ 0,0 };
		GetAbsolutePosition(&Abs);
		D3DXVec2Subtract(&m_MouseClickPos, &D3DXVECTOR2{ float(x),float(y) }, &Abs);
		m_IsMouseDown = true;
	}
}

void WindowControl::OnMouseMove(int x, int y)
{
	if (m_IsMouseDown && !m_IsParentWindow)
	{
		D3DXVECTOR2 Abs{ 0,0 }, Rel{ 0,0 };
		GetAbsolutePosition(&Abs);
		D3DXVec2Subtract(&Rel, &D3DXVECTOR2{ float(x),float(y) }, &Abs);
		D3DXVec2Subtract(&m_Position, &Rel, &m_MouseClickPos);
	}
}

void WindowControl::OnMouseUp(int Button, int x, int y)
{
	if (m_IsMouseDown && !m_IsParentWindow)
	{
		m_IsMouseDown = true;
	}
}

void WindowControl::OnSetFocus()
{
}

void WindowControl::OnLostFocus()
{
}

void WindowControl::OnKeyDown(WPARAM Key, LPARAM Extended)
{
}

void WindowControl::OnKeyUp(WPARAM Key, LPARAM Extended)
{
}

bool WindowControl::LoadCanvasFromFile(char * File)
{
	D3DXIMAGE_INFO l_ImageInfo;
	if (SUCCEEDED(D3DXGetImageInfoFromFile(File, &l_ImageInfo)))
	{
		SetWidthHeight(l_ImageInfo.Width, l_ImageInfo.Height);
		if (SUCCEEDED(GetTexture()->LoadFromFile(File)))
			return true;			
	}
	return false;
}
///----------------------------------------------------------------Label--------------------------------------------------------

LabelControl::LabelControl(UIBase * parent, int vecPos, LOGFONT Font, RECT Rect,  LPDIRECT3DDEVICE9 Device): UIBase(parent,vecPos), m_Rect(Rect)
{
	D3DXCreateFont(Device, Font.lfWidth, Font.lfHeight, Font.lfWeight, 1, Font.lfItalic,
		Font.lfCharSet, Font.lfOutPrecision, Font.lfQuality, Font.lfPitchAndFamily, Font.lfFaceName, &m_Font);
	strcpy_s(m_Caption," ");
	m_Color = D3DCOLOR_XRGB(0, 0, 0);
	m_ColorOver = D3DCOLOR_XRGB(255, 0, 0);
	m_ColorNULL = D3DCOLOR_XRGB(0, 0, 0);
	m_Format = DT_CENTER;
	m_ButtonLabel = false;
}

LabelControl::~LabelControl()
{
	if (m_Font)
		m_Font->Release();
}

bool LabelControl::OnRender()
{
	if (m_Font)
	{
		m_Font->DrawText(NULL, m_Caption, strlen(m_Caption), &m_Rect, m_Format, m_Color);
	}
	return true;
}

void LabelControl::OnMouseDown(int Button, int x, int y)
{
	if ((Button == VK_LBUTTON) && (CursorIntersect(float(x), float(y))))
	{
		//do something here
	}
}

void LabelControl::OnMouseMove(int x, int y)
{
	if (CursorIntersect(float(x), float(y)))
	{
		m_Color = m_ColorOver;
	}
	else
		m_Color = m_ColorNULL;
}

void LabelControl::OnMouseUp(int Button, int x, int y)
{
}

void LabelControl::SetCaption(char * Caption)
{
	if (Caption && *Caption)
	{
		strcpy_s(m_Caption, Caption);
	}
	else
		strcpy_s(m_Caption, " ");
}
///--------------------------------------------------------------Buttons-----------------------------------------
ButtonControl::ButtonControl(UIBase * parent, int vecPos, LPDIRECT3DDEVICE9 Device):UIBase(parent,vecPos)
{
	m_Device = Device;
	m_DefaultTex = new Texture(m_Device);
	m_OverTex - new Texture(m_Device);
	m_Over = false;
	m_Caption = NULL;
}

ButtonControl::~ButtonControl()
{
	if (m_DefaultTex)
		delete m_DefaultTex;
	if (m_OverTex)
		delete m_OverTex;
	if (m_Texture)
		delete m_Texture;
	DeleteObject(m_Font);
	if (m_Caption)
		delete m_Caption;
}

bool ButtonControl::OnRender()
{
	if (m_Over)
		SetTexture(m_OverTex);
	else
		SetTexture(m_DefaultTex);

	GetSprite()->DrawTexture(m_Texture);

	if (m_Caption)
	{
		m_Caption->OnRender();
	}
	return false;
}

void ButtonControl::OnMouseDown(int Button, int x, int y)
{
}

void ButtonControl::OnMouseMove(int x, int y)
{
	if (CursorIntersect(float(x), float(y)))
	{
		m_Over = true;
	}
	else
		m_Over = false;

}

void ButtonControl::OnMouseUp(int Button, int x, int y)
{
}

bool ButtonControl::SetTextures(char * fileDefault, char * fileOver)
{
	D3DXIMAGE_INFO l_Info;
	if (m_DefaultTex)
	{
		if (SUCCEEDED(D3DXGetImageInfoFromFile(fileDefault,&l_Info)))
		{
			SetWidthHeight(l_Info.Width, l_Info.Height);
			if (!SUCCEEDED(m_DefaultTex->LoadFromFile(fileDefault)))
				return false;
		}	
	}
	if (m_OverTex)
	{
		if (SUCCEEDED(D3DXGetImageInfoFromFile(fileOver, &l_Info)))
		{
			SetWidthHeight(l_Info.Width, l_Info.Height);
			if (!SUCCEEDED(m_OverTex->LoadFromFile(fileDefault)))
				return false;
		}
	}
	m_Rect.left = 0;
	m_Rect.top = 0;
	m_Rect.right = l_Info.Width;
	m_Rect.bottom = l_Info.Height;
	return true;
}

void ButtonControl::SetCaption(char * Caption)
{
	LOGFONT lf;
	if (m_Caption)
		delete m_Caption;

	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
	m_Caption = new LabelControl(GetThis(), 1, lf, m_Rect, m_Device);
	m_Caption->SetCaption(Caption);
}
