#include "stdafx.h"

ATOM ball_register_class(const HINSTANCE h_instance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ball_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = h_instance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = sz_ball_class;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

ATOM ball_trace_register_class(const HINSTANCE h_instance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ball_trace_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = h_instance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = sz_ball_trace_class;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

void create_trace()
{
	for (auto& i : h_trace)
	{
		if (i != nullptr)
		{
			RECT rc;
			GetWindowRgnBox(i, &rc);
			if (rc.right - rc.left > BALL_SIZING)
			{
				const HRGN h_new_region = CreateEllipticRgn(rc.left + BALL_SIZING / 2,
														  rc.top + BALL_SIZING / 2,
														  rc.right + 1 - BALL_SIZING / 2,
														  rc.bottom + 1 - BALL_SIZING / 2);
				SetWindowRgn(i, h_new_region, true);
			}
			else
			{
				DestroyWindow(i);
				i = nullptr;
			}
		}
	}
}

void update_trace(const HWND h_parent, const RECT ball_rc)
{
	last_trace = (last_trace + 1) % trace_count;
	h_trace[last_trace] = CreateWindow(sz_ball_trace_class, nullptr, WS_CHILD | WS_CLIPSIBLINGS,
									   ball_rc.left + BALL_SIZING / 2, ball_rc.top + BALL_SIZING / 2,
		ball_diameter - BALL_SIZING, ball_diameter - BALL_SIZING, h_parent, nullptr, h_inst,
									   nullptr);
	const HRGN region = CreateEllipticRgn(BALL_SIZING / 2, BALL_SIZING / 2, ball_diameter - BALL_SIZING / 2,
										  ball_diameter - BALL_SIZING / 2);
	SetWindowRgn(h_trace[last_trace], region, true);
	ShowWindow(h_trace[last_trace], SW_SHOW);
}

void game(const HWND h_wnd, const HWND h_parent, RECT& ball_rc, const RECT paddle_rc)
{
	ball_rc.left += DISTANCE * direction.x;
	ball_rc.right += DISTANCE * direction.x;
	ball_rc.top += DISTANCE * direction.y;
	ball_rc.bottom += DISTANCE * direction.y;
	ball_rc = collision_detection(h_parent, ball_rc, paddle_rc);
	if (!is_loose)
	{
		MoveWindow(h_wnd, ball_rc.left, ball_rc.top, ball_diameter, ball_diameter, true);
	}
	is_loose = false;
}

LRESULT CALLBACK ball_proc(const HWND h_wnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(h_wnd, firsttimer, TIME, nullptr);
		SetTimer(h_wnd, secondtimer, tracetime, nullptr);
		direction = START_DIRECTION;
	}
	break;
	case WM_TIMER:
	{
		const HWND h_parent = GetParent(h_wnd);
		RECT ball_rc = pos_on_parent(h_wnd, h_parent);
		const RECT paddle_rc = pos_on_parent(h_paddle, h_parent);
		switch (w_param)
		{
		case firsttimer:
			if (!is_pause)
			{
				game(h_wnd, h_parent, ball_rc, paddle_rc);
			}
			break;
		case secondtimer:
			create_trace();
			if (!is_pause)
			{
				update_trace(h_parent, ball_rc);
			}
		default:;
		}
	}
	break;
	case WM_NCHITTEST:
		return HTTRANSPARENT;
	default:
		return DefWindowProc(h_wnd, message, w_param, l_param);
	}
	return 0;
}

LRESULT CALLBACK ball_trace_proc(const HWND h_wnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
	if (message == WM_NCHITTEST)
		return HTTRANSPARENT;

	return DefWindowProc(h_wnd, message, w_param, l_param);
}
