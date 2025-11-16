
#define UNICODE
#define _UNICODE

#include<windows.h>
#include<thread>
// #include<string>

long long CALLBACK WndProc(HWND__ * hwnd , unsigned int msg, unsigned long long wParam , long long lParam)
{
    switch(msg)
    {
        case 0x000F:
        {
            tagPAINTSTRUCT ps;
            HDC__ * hdc = BeginPaint(hwnd , &ps);
            const wchar_t* text = L"MinGW Win32 programming...";
            TextOutW(hdc ,100 ,100 ,text,26);
            EndPaint(hwnd , &ps);
            return 0;
        }
        case 0x0002:
        {
            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProcW(hwnd, msg , wParam , lParam);

    }
}

int RegisterWindowClass(HINSTANCE__ * hInstance)
{
    WNDCLASSEXW wc_exw = {0};
    wc_exw.cbSize = sizeof(WNDCLASSEXW);
    wc_exw.lpfnWndProc = WndProc;
    wc_exw.hInstance = hInstance;
    wc_exw.lpszClassName = L"MyWindowClass";
    wc_exw.hbrBackground = (HBRUSH__*)(COLOR_WINDOW);
    wc_exw.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc_exw.hIcon = LoadIconW(NULL , IDI_APPLICATION);

    return RegisterClassExW(&wc_exw) !=0;
}

HWND__ * CreateMyWindow(HINSTANCE__ * hInstance)
{
    return CreateWindowExW
    (
        0,
        L"MyWindowClass",
        L"Hello Win32",
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

int WINAPI wWinMain(HINSTANCE__ * hInstance, HINSTANCE__ * hPrevInstance ,wchar_t * pCmdLine , int nCmdShow)
{

    if(!RegisterWindowClass(hInstance))
    {
        MessageBox(NULL,L"Window Register is wrong" , L"Wrong",MB_ICONERROR);
        return 0;
    }

    HWND__ * hwnd = CreateMyWindow(hInstance);
    if(!hwnd)
    {
        MessageBox(NULL,L"Window Creatint is wrong...",L"Wrong",MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd , nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        Sleep(10);
    }

    return (int)msg.wParam;
}

