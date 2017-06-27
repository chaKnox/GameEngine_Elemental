#include "Input.h"
#include <string>
#define KEYDOWN(name,key) (name[key] & 0x80)



///------------------------------------------------------------------Input------------------------------------------------------------------------
Input::Input( HINSTANCE hinst, HWND hWnd )
{
    m_pInput = NULL;
    m_hWnd = hWnd;
    DirectInput8Create( hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL );
}

Input::~Input( )
{
    if( m_pInput != NULL )
        m_pInput->Release( );
}

Keyboard * Input::CreateKeyboard( )
{
    Keyboard* l_keyboard = NULL;

    if( m_pInput )
    {
        l_keyboard = new Keyboard( m_pInput, m_hWnd );
    }
    return l_keyboard;
}

Mouse * Input::CreateMouse( LPDIRECT3DDEVICE9 pDevice, bool Exclusive )
{
    Mouse* l_mouse = NULL;
    if( m_pInput )
    {
        l_mouse = new Mouse( pDevice, m_pInput, m_hWnd, Exclusive );
    }
    return l_mouse;
}
Joystick * Input::CreateJoystick( LPDIRECT3DDEVICE9 pDevice, bool Exclusive )
{
    Joystick* l_Joystick = NULL;
    if( m_pInput )
    {
        l_Joystick = new Joystick( pDevice, m_pInput, m_hWnd, Exclusive );
    }
    return l_Joystick;
}
///----------------------------------------------------------------------Keyboard--------------------------------------------------------------------
Keyboard::Keyboard( LPDIRECTINPUT8 pInput, HWND hWnd )
{
    HRESULT Result = E_FAIL;
    m_pInputDevice = NULL;
    Result = pInput->CreateDevice( GUID_SysKeyboard, &m_pInputDevice, NULL );
    if( SUCCEEDED( Result ) )
    {
        Result = m_pInputDevice->SetDataFormat( &c_dfDIKeyboard );
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }
        Result = m_pInputDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }
        memset( m_KeyState, 0, 256 * sizeof( char ) );
    }
}

Keyboard::~Keyboard( )
{
    m_pInputDevice->Unacquire( );
    SafeRelease( m_pInputDevice );
}

bool Keyboard::IsKeyPressed( int Key )
{
    if( m_pInputDevice )
    {
        if( m_KeyState[ Key ] & 0x80 )
            return true;
    }
    return false;
}

HRESULT Keyboard::Update( )
{
    HRESULT Result = E_FAIL;
    if( m_pInputDevice )
    {
        Result = m_pInputDevice->Acquire( );
        if( FAILED( Result ) )
            return Result;
        Result = m_pInputDevice->GetDeviceState( sizeof( m_KeyState ), (LPVOID)&m_KeyState );
        if( FAILED( Result ) )
            return Result;
    }
    return S_OK;
}
///----------------------------------------------------------------Mouse-------------------------------------------------------------------------------
Mouse::Mouse( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
              bool Exclusive/*, D3DDISPLAYMODE Mode*/ ): m_pInputDevice( NULL )
{
    m_Device = NULL;
    //Initial cursor position
    m_iX = 0;
    m_iY = 0;
    //int temp = 0;
    //ZeroMemory( (void*)&pos, sizeof( &pos ) );
    //for (int i = 0; i < sizeof(pos); i++)
    //{
    //	pos[i].top = 0;
    //	pos[i].left = 0;
    //	pos[i].right += temp + 16;
    //	pos[i].bottom = 16;
    //}

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
        //m_Surf = new Surface(m_Device);
        //Image infor structure
        D3DXIMAGE_INFO ImageInfo;
        //load image iformation
        D3DXGetImageInfoFromFile( "cursor.png", &ImageInfo );
        m_Device->CreateOffscreenPlainSurface( ImageInfo.Height, ImageInfo.Width, ImageInfo.Format, D3DPOOL_DEFAULT, &m_Cursor, NULL );
        D3DXLoadSurfaceFromFile( m_Cursor, NULL, NULL, "cursor.png", NULL, D3DX_FILTER_NONE, D3DCOLOR_XRGB( 255, 255, 255 ), &ImageInfo );
        //SetMouseCursor("cursor.png", 0, 0, 0);
        m_Device->SetCursorProperties( 0, 0, m_Cursor );
        m_Device->SetCursorPosition( 0, 0, D3DCURSOR_IMMEDIATE_UPDATE );
        //m_Device->ShowCursor( true );
        //ZeroMemory( (void*)&Mode, sizeof( &Mode ) );
        //m_Device->GetDisplayMode( 0, &Mode );
        m_Changed = false;
        m_Buttons = false;
    }

}

Mouse::~Mouse( )
{
    if( m_pInputDevice )
    {
        m_pInputDevice->Unacquire( );
        SafeRelease( m_pInputDevice );
    }
    SafeRelease( m_Cursor );
    //delete m_Surf;
}

HRESULT Mouse::Update( )
{
    HRESULT Result = E_FAIL;
    long OldX, OldY;
    bool Pressed = this->IsButtonPressed( 0 );
    m_Changed = false;

    if( m_pInputDevice )
    {
        if( SUCCEEDED( m_pInputDevice->Acquire( ) ) )
        {
            Result = m_pInputDevice->GetDeviceState( sizeof( DIMOUSESTATE ), (void*)&m_State );
            if( FAILED( Result ) )
                return Result;
        }
        else
            return Result;
        Result = m_pInputDevice->Poll( );
        if( FAILED( Result ) )
            return Result;
        if( this->IsButtonPressed( 0 ) != Pressed )
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
    if( KEYDOWN( m_State.rgbButtons, Button ) )
        return true;
    else
        return false;
}


HRESULT Mouse::SetMouseCursor( char * FilePath, UINT x, UINT y, int Type )
{
    //HRESULT Result;
    //create mouse cursor
    //Result = m_Surf->LoadFromFile(FilePath);
    //m_Device->CreateOffscreenPlainSurface(16, 16, m_Surf->Info.Format, D3DPOOL_DEFAULT, &m_Cursor, NULL);
    //m_Surf->CopySurface( m_Cursor, pos[ Type ], x, y );
    //m_Device->SetCursorProperties(x, y, m_Cursor);
    return E_NOTIMPL;
}

void Mouse::SetCursorImage( int Type )
{
    //m_Surf->CopySurface(m_Cursor, pos[Type], m_iX, m_iY);
    m_Device->SetCursorProperties( m_iX, m_iX, m_Cursor );
}

void Mouse::SetCursorPosition( int x, int y )
{
    m_Device->SetCursorPosition( x, y, 0 );
}

HRESULT Mouse::SetCursorVisible( bool Show )
{
    return m_Device->ShowCursor( Show );
}
///-------------------------------------------------------------------------------------------Joystick----------------------------------------------------------------------------

Joystick::Joystick( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd, bool Exclusive )
{
    m_iX = 0, m_iY = 0;
    m_Device = pDevice;
    m_pJSDevice = NULL;
    ZeroMemory( &m_JoystickGUID, sizeof( GUID ) );
    ZeroMemory( &m_JoyState, sizeof( DIJOYSTATE ) );

    // first find the GUID of your particular joystick
    pInput->EnumDevices( DI8DEVCLASS_GAMECTRL, DI_Enum_Joysticks, this, DIEDFL_ATTACHEDONLY );
    if( FAILED( pInput->CreateDevice( m_JoystickGUID, &m_pJSDevice, NULL ) ) )
    {
        MessageBox( NULL, "Failed to create joystick", "error", NULL );
    }

    if( FAILED( m_pJSDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND ) ) )
    {
        MessageBox( NULL, "Failed to set cooperative level:  joystick", "error", NULL );
    }
    if( FAILED( m_pJSDevice->SetDataFormat( &c_dfDIJoystick ) ) )
    {
        MessageBox( NULL, "Failed to set JS data format", "error", NULL );
    }
    // set the range of the joystick
    DIPROPRANGE joy_axis_range;

    // first JS range
    joy_axis_range.lMin = -100;
    joy_axis_range.lMax = 100;

    joy_axis_range.diph.dwSize = sizeof( DIPROPRANGE );
    joy_axis_range.diph.dwHeaderSize = sizeof( DIPROPHEADER );
    joy_axis_range.diph.dwObj = DIJOFS_X | DIJOFS_Y;
    joy_axis_range.diph.dwHow = DIPH_BYOFFSET;

    m_pJSDevice->SetProperty( DIPROP_RANGE, &joy_axis_range.diph );
    // and now the dead band
    DIPROPDWORD dead_band; // here's our property word

    dead_band.diph.dwSize = sizeof( dead_band );
    dead_band.diph.dwHeaderSize = sizeof( dead_band.diph );
    dead_band.diph.dwObj = DIJOFS_X | DIJOFS_Y;
    dead_band.diph.dwHow = DIPH_BYOFFSET;
    // 10% will be used on both sides of the range +/-
    dead_band.dwData = 1000;
    // finally set the property
    m_pJSDevice->SetProperty( DIPROP_DEADZONE, &dead_band.diph );
    if( FAILED( m_pJSDevice->Acquire( ) ) )
    {
        MessageBox( NULL, "Failed to aquire joystick", "error", NULL );
    }
}

Joystick::~Joystick( )
{
}

HRESULT Joystick::Update( )
{
    HRESULT HR = NULL;
    if( m_pJSDevice )
    {
        if( SUCCEEDED( m_pJSDevice->Acquire( ) ) )
        {
            HR = m_pJSDevice->GetDeviceState( sizeof( DIJOYSTATE ), (void*)&m_JoyState );
            if( FAILED( HR ) )
                return HR;
        }
        else
            return HR;
        HR = m_pJSDevice->Poll( );
        if( FAILED( HR ) )
            return HR;
        //if( this->IsButtonPressed( 0 ) != Pressed )
        //    m_Buttons = true;
        //else
        //    m_Buttons = false;
        m_iX += (m_JoyState.lX * 0.0001f);
        m_iY -= (m_JoyState.lY * 0.0001f);
        m_Device->SetCursorPosition( m_iX, m_iY, D3DCURSOR_IMMEDIATE_UPDATE );
    }
    return HR;
}

LONG Joystick::GetXPos( )
{
    return m_iX;
}

LONG Joystick::GetYPos( )
{
    return m_iY;
}

bool Joystick::IsButtonPressed( int Button )
{
    if( m_JoyState.rgbButtons[ Button ] )
        return true;
    return false;
}

HRESULT Joystick::SetMouseCursor( char * FilePath, UINT x, UINT y, int Type )
{
    return E_NOTIMPL;
}

void Joystick::SetCursorImage( int Type )
{
}

void Joystick::SetCursorPosition( int x, int y )
{
}

HRESULT Joystick::SetCursorVisible( bool Show )
{
    return E_NOTIMPL;
}
