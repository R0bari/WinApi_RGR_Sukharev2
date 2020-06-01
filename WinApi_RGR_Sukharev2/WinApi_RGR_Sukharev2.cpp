#include <windows.h>
#include <string>
#include "Resource.h"

class Button {
public:
	HWND instance;
	HMENU btnClick;
	LONG left, top, right, bottom;
	Button(HINSTANCE hInstance, HWND hWnd, LPCSTR title, LONG x, LONG y, LONG width, LONG height, HMENU btnClick) {
		instance = CreateWindow((LPCSTR)"button", title, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, width, height, hWnd, btnClick, hInstance, NULL);
		left = x;
		top = y;
		right = x + width;
		bottom = y + height;
		this->btnClick = btnClick;
	}

	LONG width() {
		return right - left;
	}
	LONG height() {
		return bottom - top;
	}
};

void RegClass(char[], int, WNDPROC);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcWind(HWND, UINT, WPARAM, LPARAM);
void ChangePopup(LPCSTR, LONG, char*);

HINSTANCE hInst;
HWND hWndMain;
HWND hWndPopup;
RECT RectMain;
char ClassMainWind[] = "MainWindow";
char ClassPopup[] = "Popup";
char PopupBlack[] = "Black";
char PopupWhite[] = "White";
char PopupDarkGray[] = "Dark Gray";
char PopupLightGray[] = "Light Gray";
char PopupSystemFont[] = "System Font";
char PopupDeviceDefault[] = "Device Default";
LONG buttonWidth = 140;
LONG buttonHeight = 50;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;

	hInst = hInstance;

	if ((hWnd = FindWindow(ClassMainWind, NULL)) != NULL)
	{
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		return FALSE;
	}

	RegClass(ClassMainWind, LTGRAY_BRUSH, WndProc);
	RegClass(ClassPopup, BLACK_PEN, WndProcWind);
	RegClass(PopupBlack, BLACK_BRUSH, WndProcWind);
	RegClass(PopupWhite, WHITE_BRUSH, WndProcWind);
	RegClass(PopupDarkGray, DKGRAY_BRUSH, WndProcWind);
	RegClass(PopupLightGray, LTGRAY_BRUSH, WndProcWind);
	RegClass(PopupSystemFont, SYSTEM_FONT, WndProcWind);
	RegClass(PopupDeviceDefault, DEVICE_DEFAULT_FONT, WndProcWind);

	hWndMain =
		CreateWindow(ClassMainWind, "WinApi_RGR_Sukharev2",
			WS_OVERLAPPEDWINDOW,
			200, 200,
			1000, 800,
			NULL, NULL, hInst, NULL);
	if (hWndMain == 0) return FALSE;


	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	GetWindowRect(hWndMain, &RectMain);

	hWndPopup =
		CreateWindow(ClassPopup, "Popup",
			WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_VISIBLE, RectMain.left + 600, RectMain.top + 100, 300, 300, hWndMain, NULL,
			hInstance, NULL);
	if (hWndPopup == 0)
	{
		DestroyWindow(hWndMain);
		return FALSE;
	}
	ShowWindow(hWndPopup, nCmdShow);
	UpdateWindow(hWndPopup);

	auto buttonBlack = Button(hInst, hWndMain, "BLACK_PEN", 100, 100, buttonWidth, buttonHeight, (HMENU)BTN_BLACK);
	auto buttonWhite = Button(hInst, hWndMain, "WHITE_PEN", 100,
		buttonBlack.top + buttonBlack.height() + 20, buttonWidth, buttonHeight, (HMENU)BTN_WHITE);
	auto buttonDarkGrey = Button(hInst, hWndMain, "DKGRAY_BRUSH", 100,
		buttonWhite.top + buttonWhite.height() + 20, buttonWidth, buttonHeight, (HMENU)BTN_DKGRAY);
	auto buttonLightGrey = Button(hInst, hWndMain, "LTGRAY_BRUSH", 100,
		buttonDarkGrey.top + buttonDarkGrey.height() + 20, buttonWidth, buttonHeight, (HMENU)BTN_LTGRAY);
	auto buttonSystemFont = Button(hInst, hWndMain, "SYSTEM_FONT", 100,
		buttonDarkGrey.top + buttonDarkGrey.height() + 20, buttonWidth, buttonHeight, (HMENU)BTN_SYSTEM_FONT);
	auto buttonDeviceDefault = Button(hInst, hWndMain, "DEVICE DEFAULT", 100,
		buttonSystemFont.top + buttonSystemFont.height() + 20, buttonWidth, buttonHeight, (HMENU)BTN_DEVICE_DEFAULT_FONT);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void RegClass(char className[], int color, WNDPROC wndProc)
{
	WNDCLASS mw;
	memset(&mw, 0, sizeof(mw));
	mw.lpszClassName = className;
	mw.lpfnWndProc = (WNDPROC)wndProc;

	mw.style = CS_HREDRAW | CS_VREDRAW;
	mw.hInstance = hInst;
	mw.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	mw.hCursor = LoadCursor(NULL, IDC_ARROW);

	mw.hbrBackground = (HBRUSH)GetStockObject(color);

	mw.lpszMenuName = NULL;
	mw.cbClsExtra = 0;
	mw.cbWndExtra = 0;

	RegisterClass(&mw);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND: 
	{
		int wmId = LOWORD(wParam);
		
		/*switch (wmId) {
		case 10000: ChangePopupBG(4);
		}*/
		switch (wmId)
		{
		case BTN_BLACK:
			ChangePopup(PopupBlack, BLACK_BRUSH, PopupBlack);
			break;
		case BTN_WHITE:
			ChangePopup(PopupWhite, WHITE_BRUSH, PopupWhite);
			break;
		case BTN_DKGRAY:
			ChangePopup(PopupDarkGray, DKGRAY_BRUSH, PopupDarkGray);
			break;
		case BTN_LTGRAY:
			ChangePopup(PopupLightGray, LTGRAY_BRUSH, PopupLightGray);
			break;
		case BTN_SYSTEM_FONT:
			ChangePopup(PopupSystemFont, SYSTEM_FONT, PopupSystemFont);
			break;
		case BTN_DEVICE_DEFAULT_FONT:
			ChangePopup(PopupDeviceDefault, DEVICE_DEFAULT_FONT, PopupDeviceDefault);
		}
	}; break;
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;

		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}; break;


	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}; break;
	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}

void ChangePopup(LPCSTR title, LONG color, char* className) {
	if (hWndPopup) DestroyWindow(hWndPopup);
	hWndPopup = CreateWindow(className, title,
		WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX | WS_VISIBLE, RectMain.left + 600, RectMain.top + 100, 300, 300, hWndMain, NULL,
		hInst, NULL);

}

LRESULT CALLBACK WndProcWind(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;

		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}; break;
	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}
