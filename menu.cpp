#include "stdafx.h"

INT_PTR CALLBACK about(const HWND h_dlg, const UINT message, const WPARAM w_param, LPARAM l_param)
{
	UNREFERENCED_PARAMETER(l_param);
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(w_param) == IDOK || LOWORD(w_param) == IDCANCEL)
		{
			EndDialog(h_dlg, LOWORD(w_param));
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}

void new_game(const HWND h_wnd, const HWND h_ball)
{
	win_count = 0;
	loose_count = 0;
	direction = START_DIRECTION;
	MoveWindow(h_ball, BALL_POS_X, BALL_POS_Y, ball_diameter, ball_diameter, true);
	for (auto& i : h_trace)
	{
		if (i != nullptr)
		{
			DestroyWindow(i);
			i = nullptr;
		}
	}
	DISTANCE = startlvl;
	game_time = 0;
	is_pause = false;
	ShowWindow(h_ball, SW_SHOW);
	update_bitmap(h_wnd);
}

void background_color(const HWND h_wnd)
{
	CHOOSECOLOR cc;
	static COLORREF arr[16];
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = h_wnd;
	cc.lpCustColors = arr;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&cc))
	{
		is_bitmap = false;
		bk_color = cc.rgbResult;
		update_bitmap(h_wnd);
		const HMENU menu = GetMenu(h_wnd);
		EnableMenuItem(menu, tilebitmap, MF_GRAYED);
		EnableMenuItem(menu, strechbitmap, MF_GRAYED);
	}
}
