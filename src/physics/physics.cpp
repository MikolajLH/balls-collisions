#include "physics.h"

namespace phs
{
	Ball::Ball(const Point& center, Float radius, Float mass)
		: Circle(center, radius), mass{ mass }, velocity{}, acceleration{}
	{}

	Wall::Wall(const Point& beg, const Point& end, Float radius)
		: Stadium(beg, end, radius)
	{}

	bool resolve_static_collision(Wall& wall,Ball& ball) {
		const auto closest_cirlce = wall.closest_circle(ball.center);
		const Float dist = distance(closest_cirlce.center, ball.center);
		if (dist > wall.radius + ball.radius)
			return false;

		const Vector displacement = Vector(closest_cirlce.center, ball.center) / dist;
		const Float diff = wall.radius + ball.radius - dist;
		ball.center += displacement * diff;
		return true;
	}

	bool resolve_static_collision(Ball& ball_1,Ball& ball_2) {
		const Float dist = distance(ball_1.center, ball_2.center);
		if (dist > ball_1.radius + ball_2.radius)
			return false;
		const Vector displacement = Vector(ball_1.center, ball_2.center) / dist;
		const Float diff = ball_1.radius + ball_2.radius - dist;
		const Float mass_ratio = ball_1.mass / (ball_1.mass + ball_2.mass);

		ball_2.center += displacement * mass_ratio * diff;
		ball_1.center -= displacement * (Float(1) - mass_ratio) * diff;

		return true;
	}

	void resolve_dynamic_collision(Ball& ball_1, Ball& ball_2) {
		const auto n = Vector(ball_1.center, ball_2.center).normalize();// normalized displacement vector
		const auto t = n.perp(); // perpendicular to displacement vector

		const Float v1n = dot(n, ball_1.velocity);
		const Float v1t = dot(t, ball_1.velocity);

		const Float v2n = dot(n, ball_2.velocity);
		const Float v2t = dot(t, ball_2.velocity);

		const Float rf = 1.f;

		const Float v1np = (ball_1.mass * v1n + ball_2.mass * v2n + ball_2.mass * rf * (v2n - v1n)) / (ball_1.mass + ball_2.mass);
		const Float v2np = (ball_1.mass * v1n + ball_2.mass * v2n + ball_1.mass * rf * (v1n - v2n)) / (ball_1.mass + ball_2.mass);

		ball_1.velocity = v1np * n + v1t * t;
		ball_2.velocity = v2np * n + v2t * t;
	}

	void resolve_dynamic_collision(Wall& wall, Ball& ball) {
		auto ball_wall = Ball(wall.closest_circle(ball.center).center, wall.radius, 10000.f);
		resolve_dynamic_collision(ball_wall, ball);
	}

	void Ball::dt(Float t) {
		center += t * velocity + t * t * Float(0.5) * acceleration;
		velocity += t * acceleration;
		acceleration = {};
	}
}