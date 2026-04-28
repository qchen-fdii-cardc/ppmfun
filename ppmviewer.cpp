#include <windows.h>
#include "frame.h"

static Frame *g_frame;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        int w = g_frame->width();
        int h = g_frame->height();
        for (auto y = 0; y < h; y++)
        {
            for (auto x = 0; x < w; x++)
            {
                BYTE r = g_frame->r(x, y);
                BYTE g = g_frame->g(x, y);
                BYTE b = g_frame->b(x, y);
                SetPixel(hdc, x, y, RGB(r, g, b));
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow)
{
    if (__argc < 2)
    {
        MessageBoxA(NULL, "Usage: ppmviewer filename.ppm", "Arguments error", MB_ICONERROR);
        return -1;
    }
    const char *ppmFile = __argv[1];
    g_frame = new Frame(ppmFile);

    if (g_frame->width() == 0)
    {
        MessageBoxA(NULL, "PPM file reading error", "File error", MB_ICONERROR);
        return -2;
    }

    const char CLASS_NAME[] = "PPM Viewer";
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindowEx(
        0, CLASS_NAME, CLASS_NAME,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        g_frame->width(), g_frame->height(),
        NULL, NULL, hInst, NULL);
    ShowWindow(hWnd, nShow);
    UpdateWindow(hWnd);
    MSG m{};
    while (GetMessage(&m, NULL, 0, 0))
    {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return 0;
}