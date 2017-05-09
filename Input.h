#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
template <class T>
void SafeRelease(T*& pT)
{
    if (pT)
    {
        pT->Release();
        pT = NULL;
    }
}

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

};
#endif
