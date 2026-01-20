// cos
#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include<windows.h>

// #include <gl/wglext.h>
#include "glad/glad.h"
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001


static HWND hwnd = NULL;
static HDC hdc = NULL;
static HGLRC hglrc = NULL;

static const int width = 800;
static const int height = 600;

static const wchar_t *windowclassname = L"Windows";

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;


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

void ClearUpOpenGLWin32()
{
    if (hglrc)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hglrc);
        hglrc = NULL;
    }
    if (hdc && hwnd)
    {
        ReleaseDC(hwnd, hdc);
    }
    if (hwnd)
    {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    UnregisterClassW(windowclassname, GetModuleHandleW(NULL));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX WndClassEx = {0};
    WndClassEx.cbClsExtra = NULL;
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.cbWndExtra = NULL;
    WndClassEx.hbrBackground = NULL;
    WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClassEx.hIcon = NULL;
    WndClassEx.hIconSm = NULL;
    WndClassEx.lpfnWndProc = WndProc;
    WndClassEx.hInstance = hInstance;
    WndClassEx.lpszClassName = L"Windows";
    WndClassEx.lpszMenuName = NULL;
    WndClassEx.style = NULL;

    if (!RegisterClassEx(&WndClassEx))
    {
        MessageBox(NULL, L"窗口创建失败", L"错误", MB_OK | MB_ICONERROR);
        return -1;
    }

    hwnd = CreateWindowEx(
        0,
        windowclassname,
        L"Windows窗口",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd)
    {
        MessageBox(NULL, L"窗口创建失败", L"错误", MB_OK | MB_ICONERROR);
        return -1;
    }
    ShowWindow(hwnd, nCmdShow);

    UpdateWindow(hwnd);

    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pixel_format_descriptor = {0};
    pixel_format_descriptor.nSize =  sizeof(PIXELFORMATDESCRIPTOR);
    pixel_format_descriptor.nVersion = 1;                                                         // 版本号，固定为1
    pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // 核心标记
    pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;                                           // 颜色模式：RGBA
    pixel_format_descriptor.cColorBits = 32;                                                      // 颜色位深：32位(8R+8G+8B+8A)
    pixel_format_descriptor.cDepthBits = 24;                                                      // 深度缓冲区：24位（开启深度测试必备）
    pixel_format_descriptor.iLayerType = PFD_MAIN_PLANE;                                          // 主绘图平面，固定值

    int pixel_format = ChoosePixelFormat(hdc, &pixel_format_descriptor);

    SetPixelFormat(hdc,pixel_format , &pixel_format_descriptor);

    HGLRC temp_hglrc = wglCreateContext(hdc);

    wglMakeCurrent(hdc, temp_hglrc);

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    
    HGLRC hglrc = nullptr;
    if (wglCreateContextAttribsARB)
    {
        // 创建新的OpenGL上下文
        hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
        // 删除临时上下文
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_hglrc);
        // 设置新的上下文为当前上下文
        wglMakeCurrent(hdc, hglrc);
    }
    else
    {
        // 如果不支持wglCreateContextAttribsARB，则使用临时上下文（可能只支持旧版本OpenGL）
        hglrc = temp_hglrc;
    }
    
    // 8. 使用Glad加载OpenGL函数指针
    if (!gladLoadGL())
    {
        // 处理错误：无法加载OpenGL函数
        // 注意：如果使用临时上下文，gladLoadGL可能会失败，因为临时上下文可能不支持现代OpenGL函数
        // 因此，最好使用wglCreateContextAttribsARB创建指定版本的上下文后再加载
    }

    int success;
    success = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);

    MSG msg = {0};

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        glClear(GL_COLOR_BUFFER_BIT);

        SwapBuffers(hdc);
    }

    return (int)msg.wParam;
}
