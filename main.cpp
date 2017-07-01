#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Graphics.h"
#include "GameTimer.h"
Graphics graphics;
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    //register the window class
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,D3D::MsgProc,0,0,GetModuleHandle(NULL),NULL,NULL,NULL,NULL,"D3D",NULL };

    //RegisterClassEx(&wc);

    //create teh applications window
    //HWND hWnd = CreateWindow("D3D", "D3D", WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
    graphics.Initialized( 768, 1366, hInst );
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
    case WM_SETCURSOR:
        SetCursor( NULL );
        graphics.GetDevice( )->ShowCursor( true );
    case MM_JOY1MOVE:
    case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_KEYUP:
	case WM_KEYDOWN:
		graphics.RecvMessages(msg, wParam, lParam, NULL);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}