#include "win32softwarecursor.h"
#include <stdint.h>
#include <conio.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

bool run = true;				// if false we need shutdown main run loop
bool mouse_pressed = false;
uint16_t mouse_x = 400;
uint16_t mouse_y = 300;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
	case WM_PAINT:
	{
		// main paint func
		PAINTSTRUCT paint;
		BeginPaint(hwnd, &paint);
		HDC hdc = GetDC(hwnd);

		// fill screen
		RECT rall;
		GetClientRect(hwnd, &rall);
		FillRect(hdc, &rall, (HBRUSH)COLOR_HIGHLIGHT);

		// draw the cross
		RECT r1 = {mouse_x - 100, mouse_y - 1, mouse_x + 100, mouse_y};
		RECT r2 = {mouse_x - 3, mouse_y - 100, mouse_x - 2, mouse_y + 100};
		HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &r1, hbr);
		FillRect(hdc, &r2, hbr);
		DeleteBrush(hbr);

		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd, &paint);
		return 0;
	}

	case WM_ERASEBKGND: // disable erase background to fix flickering
		return 1;

	case WM_LBUTTONDOWN: // if left button is pressed - capture hardware cursor
		if(!mouse_pressed)
		{
			mouse_pressed = true;
			SetCapture(hwnd);
		}
		break;
	case WM_LBUTTONUP:
		mouse_pressed = false;
		ReleaseCapture();
		break;

	case WM_RBUTTONDOWN: // if right button is pressed - capture software cursor
		if(!mouse_pressed)
		{
			mouse_pressed = true;
			SetCapture(hwnd);
			win32softwarecursor(true);
		}
		break;
	case WM_RBUTTONUP:
		mouse_pressed = false;
		ReleaseCapture();
		win32softwarecursor(false);
		break;

	case WM_MOUSEMOVE: // update mouse coordinates and refresh the window
		if(mouse_pressed)
		{
			mouse_x = GET_X_LPARAM(lparam);
			mouse_y = GET_Y_LPARAM(lparam);
			InvalidateRect(hwnd, NULL, false);
		}
		break;

	case WM_KEYUP: // close window on Esc
		if(wparam == 0x1B)
			run = false;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		run = false;
		return 0;
	case WM_CLOSE:
		run = false;
		return 0;

	default:
		break;
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

int main()
{
	// register window class
	#define WIN_CLASS_NAME "win32softwarecursorexample"
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT(WIN_CLASS_NAME);
	if(!RegisterClass(&wc))
	{
		fprintf(stderr, "can't register windows class\n");
		return 1;
	}

	// create window itself
	HWND hwnd;
	if(!(hwnd =
		 CreateWindowEx(
						WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
						TEXT(WIN_CLASS_NAME),
						TEXT("LMB - hardware cursor, RMB - software cursor"),
						WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
						0, 0, 800, 600,
						NULL,
						NULL,
						GetModuleHandle(NULL),
						NULL)))
	{
		fprintf(stderr, "can't create window\n");
		return 1;
	}

	// main run loop
	while(run)
	{
		MSG msg;
		if(GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// destory window
	if(!DestroyWindow(hwnd))
	{
		fprintf(stderr, "failed to release window handle\n");
		return 1;
	}

	// unregister window class
	if(!UnregisterClass(TEXT(WIN_CLASS_NAME), GetModuleHandle(NULL)))
	{
		fprintf(stderr, "failed to unregister window class\n");
		return 1;
	}

	// disable software cursor just in case we forgot to do it somewhere else
	win32softwarecursor(false);

	return 0;
}
