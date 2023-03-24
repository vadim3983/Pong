#include "stdafx.h"


void select_item(const HWND h_wnd, const UINT u_id)
{
	const HMENU h_menu = GetMenu(h_wnd);
	UINT second_id;
	if (u_id == tilebitmap)
	{
		second_id = strechbitmap;
		is_stretched = false;
	}
	else
	{
		second_id = tilebitmap;
		is_stretched = true;
	}
	CheckMenuItem(h_menu, second_id, MF_UNCHECKED);
	CheckMenuItem(h_menu, u_id, MF_CHECKED);
	update_bitmap(h_wnd);
}

void set_bitmaps(const HWND h_wnd)
{
	RECT rc;
	GetClientRect(h_wnd, &rc);
	const HDC hdc = GetDC(h_wnd);
	if (h_old_bmp != nullptr)
	{
		SelectObject(h_mem_dc, h_old_bmp);
	}
	if (h_bmp != nullptr)
	{
		DeleteObject(h_bmp);
	}
	h_bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	h_old_bmp = static_cast<HBITMAP>(SelectObject(h_mem_dc, h_bmp));
	ReleaseDC(h_wnd, hdc);
}

void set_bitmap_counters(const HWND h_wnd)
{
	SetBkMode(h_mem_dc, TRANSPARENT);
	const HFONT h_font = CreateFont(point_height, 0, 0, 0, 0, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS,
	                                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana");
	const auto h_old_font = static_cast<HFONT>(SelectObject(h_mem_dc, h_font));
	wchar_t w_win_count[POINT_DIGITS + 1];
	wchar_t w_loose_count[POINT_DIGITS + 1];
	_itow_s(win_count, w_win_count, 10);
	_itow_s(loose_count, w_loose_count, 10);
	SetTextColor(h_mem_dc, bk_color ^ 0x00FFFFFF);

	DrawText(h_mem_dc, w_loose_count, -1, &loose_rc, DT_CENTER);
	DrawText(h_mem_dc, w_win_count, -1, &win_rc, DT_CENTER);

	SelectObject(h_mem_dc, h_old_font);
	DeleteFont(h_font);

	InvalidateRect(h_wnd, nullptr, true);
}

void stretched_bitmap(const HWND h_wnd, const RECT rc)
{
	const HDC hdc = GetDC(h_wnd);
	const HDC h_tmp_dc = CreateCompatibleDC(hdc);
	const auto h_old_tmp_bmp = static_cast<HBITMAP>(SelectObject(h_tmp_dc, h_bitmap));

	BITMAP bmp_info;
	GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);
	SetStretchBltMode(h_mem_dc, HALFTONE);
	StretchBlt(h_mem_dc, 0, 0, rc.right, rc.bottom, h_tmp_dc, 0, 0, bmp_info.bmWidth, bmp_info.bmHeight,
	           SRCCOPY);

	SelectObject(h_tmp_dc, h_old_tmp_bmp);
	DeleteDC(h_tmp_dc);
	ReleaseDC(h_wnd, hdc);
}

void update_bitmap(const HWND h_wnd)
{
	RECT rc;
	GetClientRect(h_wnd, &rc);
	if (is_bitmap)
	{
		if (is_stretched)
		{
			stretched_bitmap(h_wnd, rc);
		}
		else
		{
			const HBRUSH new_brush = CreatePatternBrush(h_bitmap);
			FillRect(h_mem_dc, &rc, new_brush);
			DeleteBrush(new_brush);
		}
	}
	else
	{
		const HBRUSH new_brush = CreateSolidBrush(bk_color);
		FillRect(h_mem_dc, &rc, new_brush);
		DeleteBrush(new_brush);
	}

	set_bitmap_counters(h_wnd);
}

void open_file(const HWND h_wnd, const OPENFILENAME ofn)
{
	const HMENU menu = GetMenu(h_wnd);
	EnableMenuItem(menu, tilebitmap, MF_ENABLED);
	EnableMenuItem(menu, strechbitmap, MF_ENABLED);
	if (h_bitmap != nullptr)
		DeleteObject(h_bitmap);
	h_bitmap = static_cast<HBITMAP>(LoadImage(nullptr, ofn.lpstrFile, IMAGE_BITMAP, 0, 0,
											  LR_LOADFROMFILE));
	is_bitmap = true;
	select_item(h_wnd, tilebitmap);
}
