#pragma once
#ifndef UICONTROLS_H
#define UICONTROLS_H
#include "UIBase.h"
class WindowControl : public UIBase
{
private:
	bool m_IsParentWindow;
	bool m_IsMouseDown;
	D3DXVECTOR2 m_MouseClickPos;
public:
	WindowControl(UIBase* parent, int vecPos);
	~WindowControl();
	bool OnRender(WPARAM Key, LPARAM Extended);
	void OnMouseDown(int Button, int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int Button, int x, int y);
	void OnSetFocus();
	void OnLostFocus();
	void OnKeyDown(WPARAM Key, LPARAM Extended);
	void OnKeyUp(WPARAM Key, LPARAM Extended);
	bool LoadCanvasFromFile(char* File);
};

//--------------------------------------------------------------------Labels---------------------------
class LabelControl : public UIBase
{
private:
	char m_Caption[255];
	LPD3DXFONT m_Font;
	D3DCOLOR m_Color;
	D3DCOLOR m_ColorOver;
	D3DCOLOR m_ColorNULL;
	DWORD m_Format;
	bool m_ButtonLabel;
	RECT m_Rect;
public:
	LabelControl(UIBase* parent, int vecPos, LOGFONT Font, RECT Rect, LPDIRECT3DDEVICE9 Device);
	~LabelControl();
	bool OnRender();
	void OnMouseDown(int Button, int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int Button, int x, int y);
	void OnKeyDown(WPARAM Key, LPARAM Extended) { return; }
	void OnKeyUp(WPARAM Key, LPARAM Extended) { return; }
	void SetCaption(char* Caption);
	char* GetCaption() { return m_Caption; }
	void SetColor(D3DCOLOR Color) { m_Color = Color; }
	void SetFormat(DWORD Format) { m_Format = Format; }
};
//-----------------------------------------------------------------Buttons------------------------------------------
class ButtonControl : public UIBase
{
	HFONT m_Font;
	LabelControl* m_Caption;
	Texture* m_DefaultTex;
	Texture* m_OverTex;
	bool m_Over;
	LPDIRECT3DDEVICE9 m_Device;
	RECT m_Rect;
public:
	ButtonControl(UIBase* parent, int vecPos, LPDIRECT3DDEVICE9 Device);
	~ButtonControl();
	bool OnRender();
	void OnMouseDown(int Button, int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int Button, int x, int y);
	void OnKeyDown(WPARAM Key, LPARAM Extended) { return; }
	void OnKeyUp(WPARAM Key, LPARAM Extended) { return; }
	bool SetTextures(char* fileDefault, char* fileOver);
	void SetCaption(char* Caption);
};
#endif // !UICONTROLS_H

