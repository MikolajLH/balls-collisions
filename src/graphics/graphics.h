#pragma once
#include <wrl.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace gfx
{
	class FactorySingleton
	{
	public:
		static ID2D1Factory& get();
	private:
		static Microsoft::WRL::ComPtr<ID2D1Factory>factory;
	};

	class WindowRenderTarget
	{
		public:
			WindowRenderTarget(HWND);
			
			void beg_draw();
			void end_draw();

			void clear(D2D1::ColorF c);

			void draw_circle(float x, float y, float r, D2D1::ColorF c);
			void fill_circle(float x, float y, float r, D2D1::ColorF c);

			void draw_elipse(float x, float y, float rx, float ry, D2D1::ColorF c);
			void fill_elipse(float x, float y, float rx, float ry, D2D1::ColorF c);

			void draw_rectangle(float x, float y, float w, float h, D2D1::ColorF c);
			void fill_rectangle(float x, float y, float w, float h, D2D1::ColorF c);

			void draw_line(float x0, float y0, float x1, float y1, D2D1::ColorF c, float t = 1.f);

			void fill_quad(
				float x1, float y1,
				float x2, float y2,
				float x3, float y3,
				float x4, float y4,
				D2D1::ColorF c);
		private:
			Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>render_target;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>solid_color_brush;
	};
}