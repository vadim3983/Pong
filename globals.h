#pragma once
#include "constants.h"

inline HINSTANCE h_inst;
inline WCHAR sz_title[MAX_LOADSTRING];
inline WCHAR sz_window_class[MAX_LOADSTRING];
inline WCHAR sz_ball_class[MAX_LOADSTRING];
inline WCHAR sz_ball_trace_class[MAX_LOADSTRING];
inline WCHAR sz_paddle_class[MAX_LOADSTRING];

inline POINT direction;
inline HWND h_paddle;
inline int win_count = 0;
inline int loose_count = 0;
inline HWND h_ball;
inline HBITMAP h_bitmap = nullptr;
inline HDC h_mem_dc = nullptr;
inline HBITMAP h_bmp = nullptr;
inline HBITMAP h_old_bmp = nullptr;
inline COLORREF bk_color = RGB(144, 238, 144);
inline bool is_bitmap = false;
inline bool is_stretched = false;
inline RECT loose_rc;
inline RECT win_rc;
inline HWND h_trace[trace_count];
inline int last_trace = 0;
inline int DISTANCE = startlvl;
inline bool is_pause = false;
inline bool is_loose = false;
inline int personal_best = 0;
inline int game_time = 0;
