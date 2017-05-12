#include "Graphics.h"



bool Graphics::Render()
{
	return false;
}

Graphics::Graphics()
{

}


Graphics::~Graphics()
{
}

bool Graphics::Initialized(int height, int width, HWND hWnd)
{
    m_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);

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

    m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_Device);

	return false;
}

void Graphics::Shutdown()
{
    if (m_Device)
    {
        m_Device->Release();
        m_Device = 0;
    }
}

bool Graphics::Frame()
{
	return false;
}

void Graphics::BeginScene(float, float, float, float)
{
}

void Graphics::EndScene()
{
}
