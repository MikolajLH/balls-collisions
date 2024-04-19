#include "graphics.h"

namespace gfx
{
	Microsoft::WRL::ComPtr<ID2D1Factory> FactorySingleton::factory;

	ID2D1Factory& FactorySingleton::get()
	{
		if (factory)return (*factory.Get());

		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			factory.GetAddressOf());
		// TODO: error checking
		return *factory.Get();
	}

	WindowRenderTarget::WindowRenderTarget(HWND hWnd) {
		RECT rect;
		GetClientRect(hWnd, &rect);

		HRESULT hr = FactorySingleton::get().CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
			render_target.GetAddressOf());

		//TODO error checking

		hr = render_target->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f), &solid_color_brush);
		// TODO: error checking
	}

	void WindowRenderTarget::beg_draw() {
		render_target->BeginDraw();
	}
	void WindowRenderTarget::end_draw() {
		render_target->EndDraw();
	}

	void WindowRenderTarget::clear(D2D1::ColorF c){
		render_target->Clear(c);
	}

	void WindowRenderTarget::draw_circle(float x, float y, float r, D2D1::ColorF c) {
		solid_color_brush->SetColor(c);
		render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), r, r), solid_color_brush.Get());
	}

	void WindowRenderTarget::fill_circle(float x, float y, float r, D2D1::ColorF c){
		solid_color_brush->SetColor(c);
		render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), r, r), solid_color_brush.Get());
	}

	void WindowRenderTarget::draw_elipse(float x, float y, float rx, float ry, D2D1::ColorF c){
		solid_color_brush->SetColor(c);
		render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), solid_color_brush.Get());
	}


	void WindowRenderTarget::fill_elipse(float x, float y, float rx, float ry, D2D1::ColorF c){
		solid_color_brush->SetColor(c);
		render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), solid_color_brush.Get());
	}

	void WindowRenderTarget::draw_rectangle(float x, float y, float w, float h, D2D1::ColorF c){
		solid_color_brush->SetColor(c);
		render_target->DrawRectangle(D2D1::RectF(x, y, x + w, y + h), solid_color_brush.Get());
	}
	void WindowRenderTarget::fill_rectangle(float x, float y, float w, float h, D2D1::ColorF c){
		solid_color_brush->SetColor(c);
		render_target->FillRectangle(D2D1::RectF(x, y, x + w, y + h), solid_color_brush.Get());
	}


	void WindowRenderTarget::draw_line(float x0, float y0, float x1, float y1, D2D1::ColorF c, float t) {
		solid_color_brush->SetColor(c);
		render_target->DrawLine(D2D1::Point2F(x0, y0), D2D1::Point2F(x1, y1), solid_color_brush.Get(), t);
	}

	void WindowRenderTarget::fill_quad(
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float x4, float y4,
		D2D1::ColorF c) {

		Microsoft::WRL::ComPtr<ID2D1PathGeometry>quad_geometry{};
		FactorySingleton::get().CreatePathGeometry(quad_geometry.GetAddressOf());
		ID2D1GeometrySink* p_sink = nullptr;
		quad_geometry->Open(&p_sink);
		p_sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
		p_sink->AddLine(D2D1::Point2F(x2, y2));
		p_sink->AddLine(D2D1::Point2F(x3, y3));
		p_sink->AddLine(D2D1::Point2F(x4, y4));
		p_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		p_sink->Close();
		p_sink->Release();
		solid_color_brush->SetColor(c);
		render_target->FillGeometry(quad_geometry.Get(), solid_color_brush.Get());
	}
}