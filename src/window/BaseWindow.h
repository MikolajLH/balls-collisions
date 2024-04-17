#pragma once
#include <cstdint>
#include <chrono>
#include <string_view>
#include <optional>
#include <print>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

namespace wnd
{
	struct MouseEvent
	{
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/about-mouse-input

		MouseEvent(HWND, UINT, WPARAM, LPARAM);
		std::int32_t window_x;
		std::int32_t window_y;

		std::int32_t screen_x;
		std::int32_t screen_y;

		bool lb_changed;
		bool is_lb_down;

		bool rb_changed;
		bool is_rb_down;
	};

	class BaseWindow
	{
		public:
			BaseWindow(std::int32_t width, std::int32_t height, std::wstring_view title);
			int run();

			[[nodiscard]] auto get_window_handle()const->HWND;
			[[nodiscard]] auto get_time()const -> float;
			[[nodiscard]] auto get_window_title()const->std::wstring_view;
			void set_window_title(std::wstring_view new_title);
		protected:
			virtual void on_update(float);
			virtual void on_create();

			virtual void on_mouse_event(MouseEvent);
		private:
			std::wstring window_title;
			HWND window_handle{};
			std::chrono::steady_clock::time_point window_loop_start;

			static [[nodiscard]] HWND create_winapi_window(
				std::int32_t width, std::int32_t height,
				std::wstring_view title,
				LRESULT(*window_procedure)(HWND, UINT, WPARAM, LPARAM), void* wm_create_lparam = nullptr);

			static std::optional<int> process_messages();

			static constexpr LPCWSTR window_class_name = L"BaseWindow_class";
			static auto window_procedure_setup(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
			static auto window_procedure_wrapper(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
			auto window_procedure(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
	};
}