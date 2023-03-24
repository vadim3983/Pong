#include "stdafx.h">

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, title, sz_title, MAX_LOADSTRING);
	LoadStringW(hInstance, ponggrlab, sz_window_class, MAX_LOADSTRING);
	LoadStringW(hInstance, ball, sz_ball_class, MAX_LOADSTRING);
	LoadStringW(hInstance, trace, sz_ball_trace_class, MAX_LOADSTRING);
	LoadStringW(hInstance, paddle, sz_paddle_class, MAX_LOADSTRING);
	my_register_class(hInstance);
	ball_register_class(hInstance);
	paddle_register_class(hInstance);
	ball_trace_register_class(hInstance);

	if (!init_instance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	const HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(ponggrlab));

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, acc_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}

ATOM my_register_class(const HINSTANCE h_instance)
{
	WNDCLASSEXW wndclassexw;

	wndclassexw.cbSize = sizeof(WNDCLASSEX);

	wndclassexw.style = CS_HREDRAW | CS_VREDRAW;
	wndclassexw.lpfnWndProc = wnd_proc;
	wndclassexw.cbClsExtra = 0;
	wndclassexw.cbWndExtra = 0;
	wndclassexw.hInstance = h_instance;
	wndclassexw.hIcon = LoadIcon(h_instance, MAKEINTRESOURCE(ponglab));
	wndclassexw.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclassexw.hbrBackground = CreateSolidBrush(bk_color);
	wndclassexw.lpszMenuName = MAKEINTRESOURCEW(ponggrlab);
	wndclassexw.lpszClassName = sz_window_class;
	wndclassexw.hIconSm = LoadIcon(wndclassexw.hInstance, MAKEINTRESOURCE(small));

	return RegisterClassExW(&wndclassexw);
}

BOOL init_instance(const HINSTANCE h_instance, const int n_cmd_show)
{
	h_inst = h_instance;

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	const HWND h_wnd = CreateWindowW(sz_window_class, sz_title,
		WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX) | WS_CLIPCHILDREN,
		(rect.left + rect.right + 1 - WIDTH) / 2, (rect.top + rect.bottom + 1 - HEIGHT) / 2, WIDTH, HEIGHT,
		nullptr, nullptr, h_instance, nullptr);

	for (auto& i : h_trace)
		i = nullptr;

	if (!h_wnd)
	{
		return FALSE;
	}

	SetWindowLong(h_wnd, GWL_EXSTYLE, GetWindowLong(h_wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(h_wnd, 0, (255 * (100 - TRANSPARENCY)) / 100, LWA_ALPHA);

	h_ball = CreateWindow(sz_ball_class, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		BALL_POS_X, BALL_POS_Y, ball_diameter, ball_diameter, h_wnd, nullptr, h_inst, nullptr);
	const HRGN region = CreateEllipticRgn(0, 0, ball_diameter, ball_diameter);
	SetWindowRgn(h_ball, region, true);

	GetClientRect(h_wnd, &rect);

	h_paddle = CreateWindow(sz_paddle_class, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		(rect.right - PADDLE_WIDTH), (rect.bottom - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT,
		h_wnd, nullptr, h_inst, nullptr);

	ShowWindow(h_wnd, n_cmd_show);
	UpdateWindow(h_wnd);

	return TRUE;
}

LRESULT CALLBACK wnd_proc(const HWND h_wnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RECT rect;
		GetClientRect(h_wnd, &rect);
		loose_rc.top = win_rc.top = POINT_TOP;
		loose_rc.bottom = win_rc.bottom = POINT_TOP + point_height;
		loose_rc.left = rect.right / 4 - point_width / 2;
		loose_rc.right = rect.right / 4 + point_width / 2;
		win_rc.left = rect.right * 3 / 4 - point_width / 2;
		win_rc.right = rect.right * 3 / 4 + point_width / 2;
		const HDC hdc = GetDC(h_wnd);
		h_mem_dc = CreateCompatibleDC(hdc);
		ReleaseDC(h_wnd, hdc);
		set_bitmaps(h_wnd);
		update_bitmap(h_wnd);
		break;
	}
	case WM_TIMER:
		if (!is_pause)
		{
			game_time += lvltime;
		}
		break;

	case WM_MOUSEMOVE:
	{
		set_paddle_pos(h_wnd, l_param);
	}
	break;
	case WM_COMMAND:
	{
		switch (const int wm_id = LOWORD(w_param))
		{
		case newgame:
			new_game(h_wnd, h_ball);
			break;
		case changebackground:
		{
			background_color(h_wnd);
		}
		break;
		case setbitmap:
		{
			OPENFILENAME openfilename;
			WCHAR file[MAX_FILENAME]{};
			ZeroMemory(&openfilename, sizeof(openfilename));
			openfilename.lStructSize = sizeof(openfilename);
			openfilename.hwndOwner = h_wnd;
			openfilename.lpstrFile = file;
			openfilename.lpstrFile[0] = '\0';
			openfilename.nMaxFile = sizeof(file);
			openfilename.lpstrFilter = L"Bitmap (*.bmp)\0*.bmp\0";
			openfilename.lpstrFileTitle = nullptr;
			openfilename.nMaxFileTitle = 0;
			openfilename.lpstrInitialDir = nullptr;
			openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&openfilename))
			{
				open_file(h_wnd, openfilename);
			}
		}
		break;
		case tilebitmap:
			{
				select_item(h_wnd, wm_id);
			}
		case strechbitmap:
		{
			select_item(h_wnd, wm_id);
		}
		break;
			case help:
			{

			}
		case IDM_ABOUT:
			DialogBox(h_inst, MAKEINTRESOURCE(IDD_ABOUTBOX), h_wnd, about);
			break;
		case IDM_EXIT:
			DestroyWindow(h_wnd);
			break;
		default:
			return DefWindowProc(h_wnd, message, w_param, l_param);
		}
	}
	break;
	case WM_ERASEBKGND:
		return 1;
	case WM_DISPLAYCHANGE:
		set_bitmaps(h_wnd);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT tag_paintstruct;
		const HDC hdc = BeginPaint(h_wnd, &tag_paintstruct);
		RECT rect;
		GetClientRect(h_wnd, &rect);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, h_mem_dc, 0, 0, SRCCOPY);

		EndPaint(h_wnd, &tag_paintstruct);
	}
	break;
	case WM_DESTROY:
		if (h_old_bmp != nullptr)
		{
			SelectObject(h_mem_dc, h_old_bmp);
		}
		if (h_mem_dc != nullptr)
		{
			DeleteDC(h_mem_dc);
		}
		if (h_bmp != nullptr)
		{
			DeleteObject(h_bmp);
		}
		if (h_bitmap != nullptr)
			DeleteObject(h_bitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(h_wnd, message, w_param, l_param);
	}
	return 0;
}

RECT pos_on_parent(const HWND h_wnd, const HWND h_parent)
{
	RECT rect;
	GetWindowRect(h_wnd, &rect);
	POINT point = { rect.left, rect.top };
	POINT corner = { rect.right, rect.bottom };
	ScreenToClient(h_parent, &point);
	ScreenToClient(h_parent, &corner);
	rect.left = point.x;
	rect.top = point.y;
	rect.right = corner.x;
	rect.bottom = corner.y;
	return rect;
}
