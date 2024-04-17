#include "window/BaseWindow.h"
#include "graphics/graphics.h"
#include "physics/geometry2d.h"


class DemoWindow : public wnd::BaseWindow
{
	public:
		gfx::WindowRenderTarget target;
		float cx = 400.f;
		float cy = 300.f;

		gm2d::Circle lb_c, rb_c;

		DemoWindow()
			: BaseWindow(800, 600, L"Demo"), target(get_window_handle()) {

			lb_c.radius = 50.f;
			rb_c.radius = 50.f;
			run();
		}


		void on_update(float et)override {
			target.beg_draw();
			target.clear(D2D1::ColorF::AliceBlue);

			target.fill_circle(cx + std::sinf(get_time()) * 300.f, cy, 20.f, D2D1::ColorF::Aquamarine);

			draw(gm2d::DirectionVector(get_time()) * 100.f, { 400.f, 300.f });

			draw(lb_c, Color::Blue);
			draw(rb_c, Color::Green);

			auto L1 = gm2d::Line(lb_c.center, gm2d::Point{ cx, cy });
			auto L2 = gm2d::Line(rb_c.center, gm2d::Point());

			draw(L1, Color::BlueViolet);
			draw(L2, Color::Black);

			for (auto p : gm2d::lines_intersection(L1, L2))
				draw(p);

			target.end_draw();
		}

		void on_mouse_event(wnd::MouseEvent me)override {
			if (me.lb_changed and me.is_lb_down) {
				lb_c.center = { (float)me.window_x, (float)me.window_y };
			}
			if (me.rb_changed and me.is_rb_down) {
				rb_c.center = { (float)me.window_x, (float)me.window_y };
			}
		}
		using Color = D2D1::ColorF;
		
		
		void draw(const gm2d::Point& p, Color c = Color::Red, float r = 5.f) {
			target.fill_circle(p.x, p.y, r, c);
		}

		void draw(const gm2d::Vector& v,const gm2d::Point& beg, Color c = Color::Black) {
			const auto end = beg + v;
			target.draw_line(beg.x, beg.y, end.x, end.y, c);
			draw(beg);
			const auto l_arrow = end + (v.normalize() * 7.f).rotate(gm2d::rad(+150.f));
			const auto r_arrow = end + (v.normalize() * 7.f).rotate(gm2d::rad(-150.f));
			target.draw_line(end.x, end.y, l_arrow.x, l_arrow.y, c);
			target.draw_line(end.x, end.y, r_arrow.x, r_arrow.y, c);
		}

		void draw(const gm2d::Circle& c, Color col = Color::Black) {
			draw(c.center);
			target.draw_circle(c.center.x, c.center.y, c.radius, col);
		}

		void draw(const gm2d::Line& line, Color c) {
			const float wbx = 0.f;
			const float wby = 0.f;

			const float wex = 2.f * cx;
			const float wey = 2.f * cy;

			float bx = std::max(line.get_x_for_y(wby), wbx);
			float by = line.get_y_for_x(bx);

			float ex = std::min(line.get_x_for_y(wey), wex);
			float ey = line.get_y_for_x(ex);


			target.draw_line(bx, by, ex, ey, c, 2.f);
			draw(gm2d::Point(bx, by), Color::Green, 10.f);
			draw(gm2d::Point(ex, ey), Color::Green, 10.f);
		}

};


int main()
{
	auto wnd = DemoWindow();
}