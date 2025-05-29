#include <Windows.h>
#include <VersionHelpers.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_TIMER) {
        SetWindowDisplayAffinity(hwnd, 0x11); // WDA_EXCLUDEFROMCAPTURE
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hins, HINSTANCE, LPSTR, int show)
{
    const wchar_t CLASS[] = L"overlay";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hins;
    wc.lpszClassName = CLASS;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        CLASS, L"",
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hins, NULL
    );

    // Make it black and opaque
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    // Set display affinity
    BOOL success = FALSE;
    if (IsWindows10OrGreater()) {
        success = SetWindowDisplayAffinity(hwnd, 0x11); // WDA_EXCLUDEFROMCAPTURE
    }
    if (!success) {
        SetWindowDisplayAffinity(hwnd, WDA_MONITOR);
    }

    ShowWindow(hwnd, show);

    // Fill with black
    HDC hdc = GetDC(hwnd);
    HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
    RECT rect = {0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)};
    FillRect(hdc, &rect, hBrush);
    ReleaseDC(hwnd, hdc);
    DeleteObject(hBrush);

    // Periodically reapply display affinity
    SetTimer(hwnd, 1, 1000, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
