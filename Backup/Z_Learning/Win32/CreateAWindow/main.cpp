// 必须定义 UNICODE 和 _UNICODE 宏，启用宽字符 API
#define UNICODE
#define _UNICODE

#include <windows.h>  // Win32 API 头文件

// 窗口过程：处理所有消息（回调函数）
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {  // 窗口重绘
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // 绘制文字（宽字符版本：L"..."）
            const wchar_t* text = L"MinGW Win32 programming...";
            TextOutW(hdc, 50, 50, text, 26);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:  // 窗口关闭
            PostQuitMessage(0);  // 发送退出消息
            return 0;

        default:  // 其他消息交给系统默认处理
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

// 注册窗口类
BOOL RegisterWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc   = WndProc;        // 绑定窗口过程
    wc.hInstance     = hInstance;      // 实例句柄
    wc.lpszClassName = L"MinGWWindowClass";  // 窗口类名（唯一）
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 背景色
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW); // 光标
    wc.hIcon         = LoadIconW(NULL, IDI_APPLICATION); // 图标

    // 注册窗口类（宽字符版本：RegisterClassExW）
    return RegisterClassExW(&wc) != 0;
}

// 创建窗口
HWND CreateMyWindow(HINSTANCE hInstance) {
    return CreateWindowExW(
        0,                          // 扩展风格
        L"MinGWWindowClass",        // 窗口类名（与注册时一致）
        L"MinGW 窗口示例",          // 窗口标题
        WS_OVERLAPPEDWINDOW,        // 窗口风格（带标题栏、最大化按钮等）
        CW_USEDEFAULT, CW_USEDEFAULT,  // 位置（默认）
        800, 600,                     // 宽高
        NULL, NULL, hInstance, NULL
    );
}

// 程序入口（GUI 程序入口为 WinMain）
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
) {
    // 1. 注册窗口类
    if (!RegisterWindowClass(hInstance)) {
        MessageBoxW(NULL, L"窗口类注册失败！", L"错误", MB_ICONERROR);
        return 0;
    }

    // 2. 创建窗口
    HWND hwnd = CreateMyWindow(hInstance);
    if (!hwnd) {
        MessageBoxW(NULL, L"窗口创建失败！", L"错误", MB_ICONERROR);
        return 0;
    }

    // 3. 显示并更新窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 4. 消息循环（核心）
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}