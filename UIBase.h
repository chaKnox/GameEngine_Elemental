#pragma once
#ifndef UIBASE_H
#define UIBASE_H
#include <d3d9.h>
#include <d3dx9.h>
#include "UIWrappers.h"
#include <vector>
#include "Client.h"


class UIBase
{
protected:
    D3DXVECTOR2 m_Position;//the position relative to the contolls parent top left corner position
    DWORD m_Width;//width of texture
    DWORD m_Height;//height of texture
    bool m_Visible;//texture visibility
    Texture* m_Texture;//the texture
    Sprite* m_Sprite;//used to draw the texture
    std::vector<UIBase*> m_vControl;//a vector of contols, the zero position is a pointer to the parent control
	int m_ChildCount;
    int m_thisVecPos;//the position of this instance in it's parents control
    bool m_Focus;//is this the focus
    UIBase* m_pUIFocus;//this is a pointer to the contol that has the keyboard focus
	Client m_MUClient;
public:
    Texture* GetTexture( void ) { return m_Texture; }
	void SetTexture(Texture* tex);
    bool GetVisible( void ) { return m_Visible; }
    void SetVisible( bool isVis ) { m_Visible = isVis; }

    Sprite* GetSprite( ) { return m_Sprite; }
    void SetSprite( Sprite* Sprite ) { m_Sprite = Sprite; }

    DWORD GetWidth( void ) { return m_Width; }
    DWORD GetHeight( void ) { return m_Height; }
    void SetWidth( DWORD newWidth ) { m_Width = newWidth; }
    void SetHeight( DWORD newHeight ) { m_Height = newHeight; }
	void SetWidthHeight(DWORD width, DWORD height) { m_Width = width; m_Height = height; }
    D3DXVECTOR2* GetPosition( void ) { return &m_Position; }
    void SetPosition( D3DXVECTOR2 setPosition ) { m_Position = setPosition; }
    void GetAbsolutePosition( D3DXVECTOR2* Position );

    bool CursorIntersect( FLOAT x, FLOAT y );
    virtual void OnMouseDown( int Button, int x, int y ) = NULL;
    virtual void OnMouseMove( int x, int y ) = NULL;
    virtual void OnMouseUp( int Button, int x, int y ) = NULL;

    virtual void OnKeyDown( WPARAM Key, LPARAM Extended ) = NULL;
    virtual void OnKeyUp( WPARAM key, LPARAM Extended ) = NULL;
	virtual bool OnRender() = NULL;

	virtual void OnLostFocus() = NULL;
    UIBase* GetFocus( ) { return m_pUIFocus; }
    void SetFocus( UIBase* keyFocus );

    bool PostMessage( UINT msg, WPARAM wParam, LPARAM lParam, void* Data );
    UIBase* PostToAll( UINT msg, WPARAM wParam, LPARAM lParam, void* Data );
	UIBase* PostToAllReverse(UINT msg, WPARAM wParam, LPARAM lParam, void* Data);

	UIBase* GetThis() { return this; }
	UIBase* GetParentControl( void ) { return m_vControl[ 0 ]; }
	UIBase( UIBase* parent, int vecPos );
	UIBase* AddChildControl(UIBase* child);
    ~UIBase( );
};

#endif // !UIBASE_H
