#include <windows.h>
#include <vector>

// 窗口类名（自定义）
const wchar_t* CLASS_NAME = L"CustomDrawWindow";

// 全局变量：存储窗口客户区尺寸
int clientWidth = 800;
int clientHeight = 600;

// 窗口过程（处理消息：绘制、窗口大小变化等）
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // 窗口需要重绘时触发（如窗口首次显示、被遮挡后恢复）
        case WM_PAINT: {
            PAINTSTRUCT ps;
            // 1. 获取窗口DC（画布）
            HDC hdc = BeginPaint(hWnd, &ps);

            // 2. 创建内存DC（双缓冲，避免闪烁）
            HDC memDC = CreateCompatibleDC(hdc);  // 内存DC，与窗口DC兼容
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);  // 内存位图（像素容器）
            HGDIOBJ oldBmp = SelectObject(memDC, memBmp);  // 将内存位图选入内存DC

            // 3. 在内存DC上绘制图形（相当于在内存位图上修改像素）
            // 3.1 绘制背景（黑色）
            RECT clientRect = {0, 0, clientWidth, clientHeight};
            HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));  // 黑色画刷
            FillRect(memDC, &clientRect, bgBrush);
            DeleteObject(bgBrush);

            // 3.2 绘制矩形（红色，线宽2）
            HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // 红色画笔（线宽2）
            SelectObject(memDC, redPen);
            Rectangle(memDC, 100, 100, 300, 300);  // 左上角(100,100)，右下角(300,300)
            DeleteObject(redPen);

            // 3.3 绘制线条（蓝色，线宽1）
            HPEN bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
            SelectObject(memDC, bluePen);
            MoveToEx(memDC, 400, 200, NULL);  // 起点(400,200)
            LineTo(memDC, 600, 400);          // 终点(600,400)
            DeleteObject(bluePen);

            // 3.4 直接操作像素（画一个绿色点）
            SetPixel(memDC, 500, 300, RGB(0, 255, 0));  // (500,300)位置画绿色点

            // 4. 将内存DC的内容（已绘制好的图形）拷贝到窗口DC（显示到屏幕）
            BitBlt(
                hdc,                // 目标DC（窗口）
                0, 0,               // 目标位置（窗口左上角）
                clientWidth, clientHeight,  // 拷贝尺寸
                memDC,              // 源DC（内存）
                0, 0,               // 源位置（内存左上角）
                SRCCOPY             // 拷贝方式（直接复制）
            );

            // 5. 释放资源
            SelectObject(memDC, oldBmp);  // 恢复内存DC的原始状态
            DeleteObject(memBmp);
            DeleteDC(memDC);
            EndPaint(hWnd, &ps);
            return 0;
        }

        // 窗口大小变化时，更新客户区尺寸
        case WM_SIZE: {
            clientWidth = LOWORD(lParam);  // 低16位是宽度
            clientHeight = HIWORD(lParam); // 高16位是高度
            return 0;
        }

        // 窗口关闭时退出程序
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    // 其他消息交给系统默认处理
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 注册窗口类并创建窗口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. 注册窗口类
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;    // 窗口消息处理函数
    wc.hInstance = hInstance;       // 应用实例句柄
    wc.lpszClassName = CLASS_NAME;  // 窗口类名
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 背景刷（默认）
    RegisterClass(&wc);

    // 2. 创建窗口
    HWND hWnd = CreateWindowEx(
        0,                          // 扩展样式
        CLASS_NAME,                 // 窗口类名
        L"手动绘制图形到窗口",       // 窗口标题
        WS_OVERLAPPEDWINDOW,        // 窗口样式（可调整大小、带标题栏等）
        CW_USEDEFAULT, CW_USEDEFAULT,  // 窗口位置（默认）
        clientWidth, clientHeight,    // 窗口大小
        NULL,                       // 父窗口
        NULL,                       // 菜单
        hInstance,                  // 应用实例
        NULL                        // 额外数据
    );

    if (hWnd == NULL) {
        return 0;
    }

    // 3. 显示窗口
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 4. 消息循环（处理窗口事件：鼠标、键盘、重绘等）
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);  // 转换键盘消息（如按键到字符）
        DispatchMessage(&msg);   // 分发消息到窗口过程
    }

    return 0;
}