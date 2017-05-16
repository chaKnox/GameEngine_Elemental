#include <d3d9.h>
#include <d3dx9.h>
#include "UIWrappers.h"
#include "Graphics.h"

Graphics graphics;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    //register the window class
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,D3D::MsgProc,0,0,GetModuleHandle(NULL),NULL,NULL,NULL,NULL,"D3D",NULL };

    //RegisterClassEx(&wc);

    //create teh applications window
    //HWND hWnd = CreateWindow("D3D", "D3D", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
    graphics.Initialized( 500, 500, hInst );
    //ShowWindow(hWnd, SW_SHOWDEFAULT);
    //UpdateWindow(hWnd);
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    //enter the message loop
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
       
            graphics.Render();
    }
    //UnregisterClass("D3D", wc.hInstance);
    return 0;
}

LRESULT WINAPI D3D::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}