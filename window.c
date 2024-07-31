#include "input.c"

static inline double win32_current_time_in_seconds() {
    LARGE_INTEGER frequency, counter;
    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter)) {
        return -1.0;
    }
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

LRESULT CALLBACK win32_wnd_proc(
	HWND window,
	UINT message,
	WPARAM w_param,
	LPARAM l_param)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			// a rather odd method but it works
			UINT virtual_key = (UINT)(uint64_t)(void*)w_param;
			int* input_key = win32_vkey_to_key(virtual_key);

			if (input_key) *input_key = 1;

			return DefWindowProc(window, message, w_param, l_param);
		}

		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO min_max_info = (LPMINMAXINFO)l_param;
			min_max_info->ptMinTrackSize.x = 800;
			min_max_info->ptMinTrackSize.y = 600;
			return DefWindowProc(window, message, w_param, l_param);
		}
		case WM_SIZE:
		{
			RECT client_rect;
			GetClientRect(window, &client_rect);

			win32_update_draw_rect(&client_rect);

			return DefWindowProc(window, message, w_param, l_param);
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return DefWindowProc(window, message, w_param, l_param);
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return DefWindowProc(window, message, w_param, l_param);
		}
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			RECT client_rect;
			GetClientRect(window, &client_rect);

			PatBlt(DeviceContext, X, Y, Width, Height, BLACKNESS);
			win32_update_window(DeviceContext);

			EndPaint(window, &Paint);
			return DefWindowProc(window, message, w_param, l_param);
		}
		default:
		{
			return DefWindowProc(window, message, w_param, l_param);
		}
	}
}

static inline HWND win32_create_window(HINSTANCE instance)
{
	WNDCLASS wnd_class = { 0 };
	wnd_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wnd_class.hInstance = instance;
	wnd_class.lpszClassName = "WindowClass";
	wnd_class.lpfnWndProc = win32_wnd_proc;

	if (!RegisterClass(&wnd_class)) return 0;

	return CreateWindowEx(
		0,
		wnd_class.lpszClassName,
		"Tetris",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		0,
		0,
		instance,
		0);

}
