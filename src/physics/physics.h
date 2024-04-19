#pragma once
#include "geometry2d.h"

namespace phs
{
	using namespace gm2d;
	class Ball : public Circle
	{
	public:
		Ball(const Point& center, Float radius, Float mass = Float(1));
		Float mass;
		Vector velocity;
		Vector acceleration;

		void dt(Float t);
	};

	class Wall : public Stadium
	{
	public:
		Wall(const Point& beg, const Point& end, Float radius);
		
	};

	bool resolve_static_collision(Wall&, Ball&);
	bool resolve_static_collision(Ball&, Ball&);

	void resolve_dynamic_collision(Ball&, Ball&);
	void resolve_dynamic_collision(Wall&, Ball&);
}