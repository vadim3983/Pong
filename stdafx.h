// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "resource.h"
#include <windowsx.h>
#include <cmath>
#include <windows.h>
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "globals.h"
#include "pong.h"


ATOM my_register_class(HINSTANCE h_instance);
BOOL init_instance(HINSTANCE, int);
LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ball_proc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);
LRESULT CALLBACK paddle_proc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);
LRESULT CALLBACK ball_trace_proc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);
ATOM ball_register_class(HINSTANCE h_instance);
ATOM paddle_register_class(HINSTANCE h_instance);
ATOM ball_trace_register_class(HINSTANCE h_instance);
RECT collision_detection(HWND h_parent, RECT ball_rc, RECT paddle_rc);
RECT pos_on_parent(HWND h_wnd, HWND h_parent);
void set_paddle_pos(HWND h_wnd, LPARAM l_param);
void new_game(HWND h_wnd, HWND h_ball);
void select_item(HWND h_wnd, UINT u_id);
void set_bitmaps(HWND h_wnd);
void update_bitmap(HWND h_wnd);
void open_file(HWND h_wnd, OPENFILENAME ofn);
void background_color(HWND h_wnd);
