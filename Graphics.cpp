#include "Graphics.h"
//#include "UIWrappers.h"

//////-------------------------------------------------------------------------D3D Namespace--------------------------------------------------
D3DMATERIAL9 D3D::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 l_Mtrl;
	l_Mtrl.Ambient = a;
	l_Mtrl.Diffuse = d;
	l_Mtrl.Specular = s;
	l_Mtrl.Emissive = e;
	l_Mtrl.Power = p;
	return l_Mtrl;
}

D3DLIGHT9 D3D::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 l_Light;
	ZeroMemory(&l_Light, sizeof(D3DLIGHT9));

	l_Light.Type = D3DLIGHT_DIRECTIONAL;
	l_Light.Ambient = *color;
	l_Light.Diffuse = *color;
	l_Light.Specular = *color;
	l_Light.Direction = *direction;

	return l_Light;
}

D3DLIGHT9 D3D::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 l_Light;
	ZeroMemory(&l_Light, sizeof(D3DLIGHT9));

	l_Light.Type = D3DLIGHT_SPOT;
	l_Light.Diffuse = *color;
	l_Light.Specular = *color;
	l_Light.Position = *position;
	l_Light.Direction = *direction;
	l_Light.Range = 1000.0f;
	l_Light.Attenuation0 = 0.0f;
	l_Light.Attenuation1 = 0.125f;
	l_Light.Attenuation2 = 0.0f;
	l_Light.Phi = D3DXToRadian(40.0f);
	l_Light.Theta = D3DXToRadian(20.0f);
	l_Light.Falloff = 1.0f;

	return l_Light;
}

D3DLIGHT9 D3D::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
{
	D3DLIGHT9 l_Light;
	ZeroMemory(&l_Light, sizeof(D3DLIGHT9));

	l_Light.Type = D3DLIGHT_POINT;
	l_Light.Ambient = *color * 0.5f;
	l_Light.Diffuse = *color * 0.5f;
	l_Light.Specular = *color;
	l_Light.Position = *position;
	l_Light.Range = 100.0f;
	l_Light.Attenuation0 = 0.0f;
	l_Light.Attenuation1 = 0.125f;
	l_Light.Attenuation2 = 0.0f;

	return l_Light;
}


float D3D::Lerp(float a, float b, float t)
{
	return ((1 - t)*a + t*b);
}
//--------------------------------------------------------------------------Graphics Class-------------------------------------------------------
bool Graphics::Render()
{
    m_Mouse->Update( );
    m_Keyboard->Update( );
    //m_Joystick->Update( );
   //clear back buffer
    m_Device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0F, 0 );

    //begin the scene
    if( SUCCEEDED( m_Device->BeginScene( ) ) )
    {
		//m_MM->Render();
		m_Gotham->Render();
        //sprt->DrawTexture( tex );
        //end the scene
        m_Device->EndScene( );
    }
    //present the back buffer contents to the display
    m_Device->Present( NULL, NULL, NULL, NULL );
	return false;
}

Graphics::Graphics()
{
    m_Device = NULL;
    m_D3DInterface = NULL;
    tex = NULL;
    //sprt = NULL;
}


Graphics::~Graphics()
{
}

bool Graphics::Initialized(int height, int width, HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW ;
    wc.lpfnWndProc = (WNDPROC)D3D::MsgProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "D3DApp";

    if (!RegisterClass(&wc))
        return false;


    HWND hWnd = CreateWindow("D3DApp", "Omni Kitty", WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX , 100, 100, width, height, 0, 0, hInstance, 0);
    if (!hWnd)
        return false;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);


    HRESULT hr = 0;



    D3DCAPS9 caps;

    m_D3DInterface = Direct3DCreate9( D3D_SDK_VERSION );

    m_D3DInterface->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

    if( !m_D3DInterface )
        return false;

    int vp = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


    D3DPRESENT_PARAMETERS d3dpp; //create a struct ot hold device ingormation

    ZeroMemory(&d3dpp, sizeof(d3dpp)); //clear structure for use
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    hr = m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device);

    if (FAILED(hr)) {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

        hr = m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device);

        if (FAILED(hr)) {
            m_D3DInterface->Release();
            return false;
        }
    }

	m_Input = new Input(hInstance, hWnd);
	m_Keyboard = m_Input->CreateKeyboard();
	m_Mouse = m_Input->CreateMouse(m_Device, false);
   // m_Joystick = m_Input->CreateJoystick( m_Device, false );
    //m_MM = new MainMenu(m_Device);
	m_Gotham = new Gotham(m_Device);
	//m_MM->Initialize();
	m_Gotham->Initialize();
    return TRUE;
}

void Graphics::Shutdown()
{
    if (m_Device)
    {
        m_Device->Release();
        m_Device = 0;
    }
    if( m_Joystick )
        delete m_Joystick;
    if (m_Keyboard)
		delete m_Keyboard;
	if (m_Mouse)
		delete m_Mouse;
    if( tex )
        delete tex;
    if( sprt )
        delete sprt;
	if (m_MM)
		delete m_MM;
	if (m_Gotham)
		delete m_Gotham;
}

bool Graphics::Frame()
{
	return false;
}

void Graphics::Update()
{
	m_Gotham->Update();
}

void Graphics::RecvMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	//m_MM->PostMessage(msg, wParam, lParam, Data);
	m_Gotham->PostMessage(msg, wParam, lParam, Data);
}
