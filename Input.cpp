#include "Input.h"
#include <string>
#define KEYDOWN(name,key) (name[key] & 0x80)



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
	Keyboard* l_keyboard = NULL;

	if (m_pInput)
	{
		l_keyboard = new Keyboard(m_pInput, m_hWnd);
	}
    return l_keyboard;
}

Mouse * Input::CreateMouse(LPDIRECT3DDEVICE9 pDevice, bool Exclusive)
{
	Mouse* l_mouse = NULL;
	if (m_pInput)
	{
		l_mouse = new Mouse(pDevice, m_pInput, m_hWnd, Exclusive);
	}
    return l_mouse;
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
///----------------------------------------------------------------Mouse-------------------------------------------------------------------------------
Mouse::Mouse( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
              bool Exclusive/*, D3DDISPLAYMODE Mode*/ )
{
    m_Device = NULL;
    //Initial cursor position
    m_iX = 0;
    m_iY = 0;

    if( pInput&&pDevice )
    {
        HRESULT Result = E_FAIL;
        m_Device = pDevice;
        Result = pInput->CreateDevice( GUID_SysMouse, &m_pInputDevice, NULL );
        if( FAILED( Result ) )
            return;
        Result = m_pInputDevice->SetDataFormat( &c_dfDIMouse );
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }
        if( Exclusive )
        {
            Result = m_pInputDevice->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND );
        }
        else
        {
            Result = m_pInputDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
        }
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }
        //ZeroMemory( (void*)&Mode, sizeof( &Mode ) );
        //m_Device->GetDisplayMode( 0, &Mode );
        m_Changed = false;
        m_Buttons = false;
    }

	ZeroMemory((void*)&pos, sizeof(&pos));
	for (int i = 0; i < sizeof(pos); i++)
	{
		pos[i].top = 0;
		pos[i].left += pos[i].right;
		pos[i].bottom += 16;
		pos[i].right += 16;
	}
	
}

Mouse::~Mouse( )
{
    if( m_pInputDevice )
    {
        m_pInputDevice->Unacquire( );
        SafeRelease( m_pInputDevice );
    }
}

HRESULT Mouse::Update( )
{
	HRESULT Result = E_FAIL;
	long OldX, OldY;
	bool Pressed = this->IsButtonPressed(0);
	m_Changed = false;

	if (m_pInputDevice)
	{
		Result = m_pInputDevice->Acquire();
		Result = m_pInputDevice->Poll();
		if (FAILED(Result))
			return Result;
		Result = m_pInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&m_State);
		if (FAILED(Result))
			return Result;
		if (this->IsButtonPressed(0) != Pressed)
			m_Buttons = true;
		else
			m_Buttons = false;
		m_iX += m_State.lX;
		m_iY += m_State.lY;
	}
    return Result;
}

LONG Mouse::GetXPos( )
{
    return m_iX;
}

LONG Mouse::GetYPos( )
{
    return m_iY;
}

bool Mouse::IsButtonPressed( int Button )
{
	if (KEYDOWN(m_State.rgbButtons, Button))
		return true;
	else
		return false;
}


HRESULT Mouse::SetMouseCursor( char * FilePath, UINT x, UINT y, int Type )
{
	HRESULT Result;
	//create mouse cursor
	m_Tex = new Texture(m_Device);
	m_Tex->LoadFromFile("cursor_sprite.png");
	Result = m_Tex->GetTexture()->GetSurfaceLevel(0, &m_Surf);
	m_Device->SetCursorProperties(x, y, m_Surf);
    return Result;
}

void Mouse::SetCursor( int Type )
{
	m_Tex->SetRect(pos[Type]);
	m_Tex->GetTexture()->GetSurfaceLevel(0, &m_Surf);
	m_Device->SetCursorProperties(m_iX, m_iY, m_Surf);
}

void Mouse::SetCursorPosition( int x, int y )
{
	m_Device->SetCursorPosition(x, y, 0);
}

HRESULT Mouse::SetCursorVisible( bool Show )
{
    return m_Device->ShowCursor(Show);
}
