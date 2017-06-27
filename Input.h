#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "UIWrappers.h"
#include <vector>
//#include "Graphics.h"
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
class Joystick;
class Input
{
private:
    LPDIRECTINPUT8 m_pInput;
    HWND m_hWnd;
public:
    Input(HINSTANCE hinst,HWND hWnd);
    Keyboard* CreateKeyboard();
    Mouse* CreateMouse(LPDIRECT3DDEVICE9 pDevice, bool Exclusive);
    Joystick* CreateJoystick( LPDIRECT3DDEVICE9 pDevice, bool Exclusive );
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
    RECT pos[8];
	//Surface* m_Surf;
	IDirect3DSurface9* m_Cursor;
    LPDIRECTINPUTDEVICE8 m_pInputDevice;
    LPDIRECT3DDEVICE9 m_Device;
    DIMOUSESTATE m_State;
    LONG m_iX;
    LONG m_iY;
    bool m_Changed;
    bool m_Buttons;
	int m_Type;
public:
    Mouse( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
           bool Exclusive/*, D3DDISPLAYMODE Mode */);
    ~Mouse( );
    HRESULT Update( );
    LONG GetXPos( );
    LONG GetYPos( );
    bool IsButtonPressed( int Button );
    HRESULT SetMouseCursor( char* FilePath,UINT x, UINT y,int Type );
    void SetCursorImage( int Type );
    void SetCursorPosition( int x, int y );
    HRESULT SetCursorVisible( bool Show );

};
///--------------------------------------------------------------------Joystick-------------------------------------------------------------
class Joystick
{
    RECT pos[ 8 ];
    //Surface* m_Surf;
    IDirect3DSurface9* m_Cursor;
    LPDIRECTINPUTDEVICE8 m_pJSDevice;
    LPDIRECT3DDEVICE9 m_Device;
    DIJOYSTATE m_JoyState;
    GUID m_JoystickGUID;
    LONG m_iX;
    LONG m_iY;
    bool m_Changed;
    bool m_Buttons;
    int m_Type;
    char m_Name[ 80 ];
    static BOOL CALLBACK DI_Enum_Joysticks
    ( LPCDIDEVICEINSTANCE lpddi,
      LPVOID DInput_ptr )
    {
        Joystick* me = static_cast<Joystick*>(DInput_ptr);

        me->m_JoystickGUID = lpddi->guidInstance;
        strcpy_s( me->m_Name, (char*)lpddi->tszProductName );

        return (DIENUM_STOP);
    }
public:
    Joystick( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
           bool Exclusive/*, D3DDISPLAYMODE Mode */ );
    ~Joystick( );
    HRESULT Update( );
    LONG GetXPos( );
    LONG GetYPos( );
    bool IsButtonPressed( int Button );
    HRESULT SetMouseCursor( char* FilePath, UINT x, UINT y, int Type );
    void SetCursorImage( int Type );
    void SetCursorPosition( int x, int y );
    HRESULT SetCursorVisible( bool Show );

};
#endif
