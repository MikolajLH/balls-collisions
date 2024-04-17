#include "BaseWindow.h"
#include <cassert>
#include <stdexcept>
#include <format>

namespace wnd
{
	BaseWindow::BaseWindow(std::int32_t width, std::int32_t height, std::wstring_view title)
		: window_title(title) {
		window_handle = create_winapi_window(width, height, title, window_procedure_setup, this);
		this->on_create();
		ShowWindow(window_handle, SW_SHOW);
	}

	int BaseWindow::run() {
		std::optional<int>exit_code{};

		float et = 0.f;

		int frames_counter = 0;
		float seconds = 0.f;
		using namespace std::literals::string_view_literals;
		static constexpr auto fmt_str{ L"{0} | FPS: {1:.2f}"sv };
		std::wstring extended_window_title(std::formatted_size(fmt_str, get_window_title(), 0.f), 0);

		window_loop_start = std::chrono::steady_clock::now();
		auto frame_end = std::chrono::steady_clock::now();
		while (not (exit_code = process_messages()).has_value()) {
			const auto frame_start = std::chrono::steady_clock::now();
			et = std::chrono::duration<float>(std::chrono::steady_clock::now() - frame_end).count();
			this->on_update(et);
		
			frames_counter += 1;
			seconds += et;
			if (seconds > 1.f) {
				const float fps = static_cast<float>(frames_counter) / seconds;
				extended_window_title.resize(std::formatted_size(fmt_str, get_window_title(), fps));
				std::format_to(extended_window_title.begin(), fmt_str, get_window_title(), fps);
				SetWindowTextW(get_window_handle(), extended_window_title.c_str());
				seconds = 0.f;
				frames_counter = 0;
			}
			frame_end = frame_start;
		}

		return exit_code.value();
	}

	HWND BaseWindow::get_window_handle()const {
		return window_handle;
	}

	float BaseWindow::get_time() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - window_loop_start).count();
	}

	std::wstring_view BaseWindow::get_window_title()const {
		return window_title;
	}

	void BaseWindow::set_window_title(std::wstring_view new_title) {
		window_title.resize(new_title.size());
		window_title.assign_range(new_title);
		SetWindowTextW(get_window_handle(), window_title.c_str());
	}

	void BaseWindow::on_create() {}

	void BaseWindow::on_update(float) {}

	void BaseWindow::on_mouse_event(MouseEvent) {}

	auto BaseWindow::create_winapi_window(
		std::int32_t w, std::int32_t h,
		std::wstring_view title,
		LRESULT(*wnd_proc)(HWND, UINT, WPARAM, LPARAM), void* wm_create_lparam) -> HWND {

		if (w <= 0 or h <= 0)
			throw std::domain_error("window dimensions have to be positive integers");

		const HINSTANCE hInstance = GetModuleHandleW(NULL);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS | CS_OWNDC;
		wc.lpfnWndProc = wnd_proc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursorW(0, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = window_class_name;
		wc.hIconSm = NULL;

		RegisterClassExW(&wc);

		RECT wr{ .left = 0, .top = 0, .right = w, .bottom = h };
		DWORD window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		AdjustWindowRect(&wr, window_style, FALSE);


		const HWND hWnd = CreateWindowExW(
			0L,
			window_class_name,
			nullptr,
			window_style,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			NULL, NULL, hInstance, wm_create_lparam);

		SetWindowTextW(hWnd, title.data());

		return hWnd;
	}

	std::optional<int> BaseWindow::process_messages() {
		MSG msg;

		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				return static_cast<int>(msg.wParam);
			}
				

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return {};
	}

	LRESULT BaseWindow::window_procedure_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		if (msg == WM_NCCREATE)
		{
			CONST CREATESTRUCTW* CONST pCreate = reinterpret_cast<CREATESTRUCTW*>(lp);
			BaseWindow* const pWnd = static_cast<BaseWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&BaseWindow::window_procedure_wrapper));

			return pWnd->window_procedure(hwnd, msg, wp, lp);
		}
		return DefWindowProcW(hwnd, msg, wp, lp);
	}

	LRESULT BaseWindow::window_procedure_wrapper(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		const auto pApp = reinterpret_cast<BaseWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		return pApp->window_procedure(hwnd, msg, wp, lp);
	}

	MouseEvent::MouseEvent(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		lb_changed = (msg == WM_LBUTTONDOWN or msg == WM_LBUTTONUP);
		is_lb_down = LOWORD(wp) & MK_LBUTTON;

		is_rb_down = LOWORD(wp) & MK_RBUTTON;
		rb_changed = (msg == WM_RBUTTONDOWN or msg == WM_RBUTTONUP);

		POINT mouse_position{};
		GetCursorPos(&mouse_position);

		screen_x = mouse_position.x;
		screen_y = mouse_position.y;
		
		ScreenToClient(hWnd, &mouse_position);

		window_x = mouse_position.x;
		window_y = mouse_position.y;
	}

	LRESULT BaseWindow::window_procedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg) {
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN: 
				SetCapture(hWnd);
				break;
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				ReleaseCapture();
				break;
			default:
				break;
		}

		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			this->on_mouse_event(MouseEvent(hWnd, msg, wp, lp));
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			[[fallthrough]];
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
	}
}