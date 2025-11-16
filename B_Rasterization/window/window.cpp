#define UNICODE
#define _UNICODE

#include <windows.h>
#include <thread>
#include "Tools.hpp"

long long __stdcall WndProc(HWND__ * hwnd , unsigned int msg , unsigned long long wParam , long long lParam)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            tagPAINTSTRUCT ps;
            HDC__ * hdc = BeginPaint(hwnd , &ps);
            const wchar_t * text = L"Raterizer";
            TextOut(hdc , 0 , 0,text ,9);
            EndPaint(hwnd , &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            return DefWindowProcW(hwnd , msg , wParam , lParam);
        }
    }
}

int RegisterWindowClass(HINSTANCE__ * hInstance)
{
    WNDCLASSEXW wc_exw = {0};
    wc_exw.cbClsExtra = 0;
    wc_exw.cbSize = sizeof(WNDCLASSEXW);
    wc_exw.cbWndExtra = 0;
    HBRUSH__ * color = CreateSolidBrush(RGB(0,0,0));
    wc_exw.hbrBackground = color;
    wc_exw.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc_exw.hIcon = LoadCursor(NULL , IDI_APPLICATION);
    wc_exw.hIconSm = NULL;
    wc_exw.hInstance = hInstance;
    wc_exw.lpfnWndProc = WndProc;
    wc_exw.lpszClassName = L"RasterizerWindow";
    wc_exw.lpszMenuName = NULL;
    wc_exw.style = NULL;
    return RegisterClassExW(&wc_exw) != 0;
}

HWND__ * CreateAWindow(HINSTANCE__ * hInstance)
{
    return CreateWindowExW
    (
        0,
        L"RasterizerWindow",
        L"XYL Resterizer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1028,
        760,
        NULL,
        NULL,
        hInstance,
        NULL
    );
}

int __stdcall wWinMain(HINSTANCE__ * hInstance , HINSTANCE__ * hPrevInstance ,wchar_t * pCmdLine , int nCmdShow)
{
    if(!RegisterWindowClass(hInstance))
    {
        printf("%dwrong",__LINE__);

        MessageBoxW(NULL,L"窗口注册失败!" , L"Wrong" , MB_ICONERROR);
        exit(0);
    }

    HWND__ * hwnd = CreateAWindow(hInstance);
    if(!hwnd)
    {
        printf("%dwrong",__LINE__);
        MessageBoxW(NULL,L"Wrong" , L"Wrong" ,MB_ICONERROR);
        exit(0);
    }

    ShowWindow(hwnd , nCmdShow);
    UpdateWindow(hwnd);

    
    tagMSG msg;

    while(GetMessageW(&msg , NULL , 0 , 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        Sleep(10);
    }

    return (int)msg.wParam;
} 