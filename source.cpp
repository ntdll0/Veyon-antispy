#include <Windows.h>

int WINAPI WinMain(HINSTANCE hins, HINSTANCE hprev, LPSTR cmd, int show)
{
	const wchar_t CLASS[] = L"overlay";
	WNDCLASS wc = {};
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hins;
	wc.lpszClassName = CLASS;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		CLASS, L"",
		WS_POPUP,
		0, 0, 
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, hins, NULL
	);

	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_COLORKEY);
	SetWindowDisplayAffinity(hwnd, WDA_MONITOR);
	ShowWindow(hwnd, show);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
