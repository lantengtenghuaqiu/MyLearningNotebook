// cos
#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "glad/glad.h"

static HWND hwnd;
static HDC hdc;
static HGLRC hglrc;

static const int width = 800;
static const int height = 600;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT print_struct;

        HDC hdc = BeginPaint(hWnd, &print_struct);

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hWnd, &print_struct);
    }
        return 0;
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
    }
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// void ClearUpOpenGLWin32(){
    
// }


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX WndClassEx;
    WndClassEx.cbClsExtra = NULL;
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.cbWndExtra = NULL;
    WndClassEx.hbrBackground = NULL;
    WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);  
    WndClassEx.hIcon = NULL;
    WndClassEx.hIconSm = NULL;
    WndClassEx.lpfnWndProc = WndProc;
    WndClassEx.hInstance = hInstance;
    WndClassEx.lpszClassName = L"HelloWindows";
    WndClassEx.lpszMenuName =NULL;
    WndClassEx.style = NULL;

    if (!RegisterClassEx(&WndClassEx))
    {
        MessageBox(NULL, L"窗口创建失败", L"错误", MB_OK | MB_ICONERROR);
        return -1;
    }


    {   

        hwnd = CreateWindowEx(
            0,
            L"HelloWindows",
            L"Windows窗口",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            NULL,
            NULL,
            hInstance,
            NULL
        );
        hdc = GetDC(hwnd);
    }

    PIXELFORMATDESCRIPTOR  pixel_format={0};
    pixel_format.nVersion      = 1;                // 版本号，固定为1
    pixel_format.dwFlags       = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // 核心标记
    pixel_format.iPixelType    = PFD_TYPE_RGBA;    // 颜色模式：RGBA
    pixel_format.cColorBits    = 32;               // 颜色位深：32位(8R+8G+8B+8A)
    pixel_format.cDepthBits    = 24;               // 深度缓冲区：24位（开启深度测试必备）
    pixel_format.iLayerType    = PFD_MAIN_PLANE;   // 主绘图平面，固定值




    ShowWindow(hwnd,nCmdShow);

    UpdateWindow(hwnd);

    // wglMakeCurrent(g_hDC, g_hRC);
    
    MSG msg = {0};
    
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        SwapBuffers(hdc);
    }

    return (int)msg.wParam;
}
