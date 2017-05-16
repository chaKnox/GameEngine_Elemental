#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "UIWrappers.h"

//template <class T>
//void SafeRelease(T*& pT)
//{
//    if (pT)
//    {
//        pT->Release();
//        pT = NULL;
//    }
//}

class Keyboard;
class Mouse;

class Input
{
private:
    LPDIRECTINPUT8 m_pInput;
    HWND m_hWnd;
public:
    Input(HINSTANCE hinst,HWND hWnd);
    Keyboard* CreateKeyboard();
    Mouse* CreateMouse(LPDIRECT3DDEVICE9 pDevice, bool Exclusive);
    ~Input();
};

///-------------------------------------------------------------Keyboard------------------------------------------------------------
class Keyboard
{
private:
    LPDIRECTINPUTDEVICE8 m_pInputDevice;
    char m_KeyState[256];
public:
    Keyboard(LPDIRECTINPUT8 pInput, HWND hWnd);
    ~Keyboard();
    bool IsKeyPressed(int Key);
    HRESULT Update();
};

///----------------------------------------------------------Mouse-------------------------------------------------------------------
class Mouse
{
    Texture* m_Tex;
    Sprite* Sprite;
    RECT pos;
    LPDIRECTINPUTDEVICE8 m_pInputDevice;
    LPDIRECT3DDEVICE9 m_Device;
    DIMOUSESTATE m_State;
    LONG m_iX;
    LONG m_iY;
    bool m_Changed;
    bool m_Buttons;
public:
    Mouse( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
           bool Exclusive, D3DDISPLAYMODE Mode );
    ~Mouse( );
    HRESULT Update( );
    LONG GetXPos( );
    LONG GetYPos( );
    bool IsButtonPressed( int Button );
    HRESULT SetCursorImage( );
    HRESULT SetMouseCursor( char* FilePath,UINT x, UINT y,int Type );
    void SetCursor( int Type );
    void SetCursorPosition( int x, int y );
    HRESULT SetCursorVisible( bool Show );

};
#endif
