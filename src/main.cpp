#include "window/BaseWindow.h"
#include "graphics/graphics.h"
#include "physics/geometry2d.h"
#include "physics/physics.h"
#include <random>
#include <ranges>

class DemoWindow : public wnd::BaseWindow
{
	public:
		gfx::WindowRenderTarget target;
		const gm2d::Point screen_middle;

		std::vector<phs::Ball> balls{};
		std::vector<D2D1::ColorF> colors;
		std::vector<phs::Wall> walls{};


		gm2d::Point impulse_end{};
		phs::Ball* f_ball = nullptr;
		

		DemoWindow(int width, int height)
			:
			BaseWindow(width, height, L"Demo"),
			target(get_window_handle()),
			screen_middle(float(width) * 0.5f, float(height) * 0.5f) {

			const float w = 300.f;
			const float h = 250.f;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(0.f, 1.f);
			for (int n = 0; n < 20; ++n)
			{
				const float radius = 5.f + dis(gen) * 25.f;
				const float x0 = screen_middle.x + w - 2.f * dis(gen) * 0.9f * w;
				const float y0 = screen_middle.y + h - 2.f * dis(gen) * 0.9f * h;
				balls.emplace_back(phs::Point{ x0, y0 }, radius, radius);

				colors.emplace_back(dis(gen), dis(gen), dis(gen));
			}
			
			walls.emplace_back(screen_middle + phs::Vector(-0.1f * w, 10.f), screen_middle + phs::Vector(0.1f * w, 50.f), 5.f);


			walls.emplace_back(screen_middle + phs::Vector(-w, h), screen_middle + phs::Vector(w, h), 10.f);

			walls.emplace_back(screen_middle + phs::Vector(-w, -h), screen_middle + phs::Vector(w, -h), 10.f);

			walls.emplace_back(screen_middle + phs::Vector(-w, h), screen_middle + phs::Vector(-w, -h), 10.f);

			walls.emplace_back(screen_middle + phs::Vector(w, h), screen_middle + phs::Vector(w, -h), 10.f);

			run();
		}


		void on_update(float et)override {

			for (auto& ball : balls) {
				ball.acceleration += {0.f, 100.f};
				ball.dt(et);
			}

			std::vector<std::pair<size_t, size_t>>ball_ball_cols{};
			std::vector<std::pair<size_t, size_t>>ball_wall_cols{};

			
			for (size_t i = 0; i < balls.size(); ++i)
				for (size_t j = i + 1; j < balls.size(); ++j)
					if (phs::resolve_static_collision(balls[i], balls[j]))
						ball_ball_cols.emplace_back(i, j);
				
					

			for (size_t i = 0; i < balls.size(); ++i)
				for (size_t j = 0; j < walls.size(); ++j)
					if (phs::resolve_static_collision(walls[j], balls[i]))
						ball_wall_cols.emplace_back(i, j);
			
			for (auto [ball_i, ball_j] : ball_ball_cols)
				phs::resolve_dynamic_collision(balls[ball_i], balls[ball_j]);

			for (auto [ball_i, wall_j] : ball_wall_cols)
				phs::resolve_dynamic_collision(walls[wall_j], balls[ball_i]);

			target.beg_draw();
			target.clear(D2D1::ColorF::AliceBlue);
			

			for (const auto& [i, ball] : std::views::enumerate(balls))
				draw(ball, colors[i]);

			for (const auto& wall : walls)
				draw(wall);


			if (f_ball) {
				POINT mp;
				GetCursorPos(&mp);
				ScreenToClient(get_window_handle(), &mp);

				target.draw_line((float)mp.x, (float)mp.y, f_ball->center.x, f_ball->center.y, Color::Red, 3.f);
			}

			target.end_draw();
		}


		void on_mouse_event(wnd::MouseEvent me)override {

			const gm2d::Point mouse_position(float(me.window_x), float(me.window_y));

			if (me.lb_changed and me.is_lb_down) {
				for (auto& ball : balls)
					if (ball.contains(mouse_position))
						f_ball = &ball;
			}
			if (me.lb_changed and not me.is_lb_down and f_ball) {
				f_ball->acceleration += gm2d::Vector(mouse_position, f_ball->center) * 100.f;
				f_ball = nullptr;
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
			//draw(c.center);
			target.fill_circle(c.center.x, c.center.y, c.radius, col);
		}

		void draw(const gm2d::LineSegment& segment, Color col = Color::Black) {
			target.draw_line(segment.beg.x, segment.beg.y, segment.end.x, segment.end.y, col);
			draw(segment.beg);
			draw(segment.end);
		}

		void draw(const gm2d::Stadium& stadium, Color col = Color::Black) {
			const auto normal = stadium.normal();

			const auto top_l = stadium.beg + normal * stadium.radius;
			const auto bot_l = stadium.beg - normal * stadium.radius;

			const auto top_r = stadium.end + normal * stadium.radius;
			const auto bot_r = stadium.end - normal * stadium.radius;

			//target.draw_line(top_l.x, top_l.y, top_r.x, top_r.y, col);

			target.fill_quad(top_l.x, top_l.y, top_r.x, top_r.y, bot_r.x, bot_r.y, bot_l.x, bot_l.y, col);
			//target.fill_quad(0.f, 0.f, 100.f, 0.f, 100.f, 100.f, 0.f, 100.f, Color::Black);
			//target.draw_line(bot_l.x, bot_l.y, bot_r.x, bot_r.y, col);

			target.fill_circle(stadium.beg.x, stadium.beg.y, stadium.radius, col);
			target.fill_circle(stadium.end.x, stadium.end.y, stadium.radius, col);
		}

		/*void draw(const gm2d::Line& line, Color c) {
			const float wbx = 0.f;
			const float wby = 0.f;


			const float wex =  width;
			const float wey = height;

			float bx = std::max(line.get_x_for_y(wby), wbx);
			float by = line.get_y_for_x(bx);

			float ex = std::min(line.get_x_for_y(wey), wex);
			float ey = line.get_y_for_x(ex);


			target.draw_line(bx, by, ex, ey, c, 2.f);
			draw(gm2d::Point(bx, by), Color::Green, 10.f);
			draw(gm2d::Point(ex, ey), Color::Green, 10.f);
		}*/

};


int main()
{
	auto wnd = DemoWindow(800, 600);
}