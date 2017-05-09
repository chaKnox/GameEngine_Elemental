#include "Input.h"




///------------------------------------------------------------------Input------------------------------------------------------------------------
Input::Input(HINSTANCE hinst, HWND hWnd)
{
    m_pInput = NULL;
    m_hWnd = hWnd;
    DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
}

Input::~Input()
{
    if (m_pInput != NULL)
        m_pInput->Release();
}

Keyboard * Input::CreateKeyboard()
{
    return nullptr;
}

Mouse * Input::CreateMouse(LPDIRECT3DDEVICE9 pDevice, bool Exclusive)
{
    return nullptr;
}
///----------------------------------------------------------------------Keyboard--------------------------------------------------------------------
Keyboard::Keyboard(LPDIRECTINPUT8 pInput, HWND hWnd)
{
    HRESULT Result = E_FAIL;
    m_pInputDevice = NULL;
    Result = pInput->CreateDevice(GUID_SysKeyboard, &m_pInputDevice, NULL);
    if (SUCCEEDED(Result))
    {
        Result = m_pInputDevice->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(Result))
        {
            SafeRelease(m_pInputDevice);
            return;
        }
        Result = m_pInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(Result))
        {
            SafeRelease(m_pInputDevice);
            return;
        }
        memset(m_KeyState, 0, 256 * sizeof(char));
    }
}

Keyboard::~Keyboard()
{
    m_pInputDevice->Unacquire();
    SafeRelease(m_pInputDevice);
}

bool Keyboard::IsKeyPressed(int Key)
{
    if (m_pInputDevice)
    {
        if (m_KeyState[Key] & 0x80)
            return true;
    }
    return false;
}

HRESULT Keyboard::Update()
{
    HRESULT Result = E_FAIL;
    if (m_pInputDevice)
    {
        Result = m_pInputDevice->Acquire();
        if (FAILED(Result))
            return Result;
        Result = m_pInputDevice->GetDeviceState(sizeof(m_KeyState), (LPVOID)&m_KeyState);
        if (FAILED(Result))
            return Result;
    }
    return S_OK;
}
