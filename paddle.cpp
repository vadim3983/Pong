#include "stdafx.h"

ATOM paddle_register_class(const HINSTANCE h_instance)
{
	WNDCLASSEXW wndclassexw;

	wndclassexw.cbSize = sizeof(WNDCLASSEX);

	wndclassexw.style = CS_HREDRAW | CS_VREDRAW;
	wndclassexw.lpfnWndProc = paddle_proc;
	wndclassexw.cbClsExtra = 0;
	wndclassexw.cbWndExtra = 0;
	wndclassexw.hInstance = h_instance;
	wndclassexw.hIcon = nullptr;
	wndclassexw.hCursor = nullptr;
	wndclassexw.hbrBackground = GetSysColorBrush(COLOR_ACTIVECAPTION);
	wndclassexw.lpszMenuName = nullptr;
	wndclassexw.lpszClassName = sz_paddle_class;
	wndclassexw.hIconSm = nullptr;

	return RegisterClassExW(&wndclassexw);
}

void set_paddle_pos(const HWND h_wnd, const LPARAM l_param)
{
	const int y_pos = GET_Y_LPARAM(l_param);
	RECT paddle_rc;
	GetWindowRect(h_paddle, &paddle_rc);
	POINT point = { paddle_rc.left, paddle_rc.top };
	ScreenToClient(h_wnd, &point);
	point.y = y_pos - PADDLE_HEIGHT / 2;
	RECT rc;
	GetClientRect(h_wnd, &rc);
	if (point.y < rc.top)
		point.y = rc.top;
	else if (point.y + PADDLE_HEIGHT > rc.bottom)
		point.y = rc.bottom - PADDLE_HEIGHT;
	MoveWindow(h_paddle, point.x, point.y, PADDLE_WIDTH, PADDLE_HEIGHT, true);
}

LRESULT CALLBACK paddle_proc(const HWND h_wnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
	if (message == WM_NCHITTEST)
		return HTTRANSPARENT;

	return DefWindowProc(h_wnd, message, w_param, l_param);
}
