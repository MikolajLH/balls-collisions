#pragma once
#include <concepts>
#include <cmath>
#include <numbers>
#include <vector>
#include <print>

namespace gm2d
{
	using Float = float;
	

	static constexpr Float inv(Float x) { return Float(1) / x; }


	bool are_nearly_equal(auto a, auto b){
		constexpr Float epsilon = Float(0.000001);
		return std::fabs(a - b) < epsilon * std::fabs(a);
	}

	bool is_nearly_zero(auto x) {
		constexpr Float epsilon = Float(0.000001);
		return std::fabs(x) < epsilon;
	}

	constexpr Float pi = std::numbers::pi_v<Float>;
	constexpr Float inv_pi = std::numbers::inv_pi_v<Float>;

	static constexpr Float deg(Float rad) noexcept { return inv_pi * Float(180) * rad; }
	static constexpr Float rad(Float deg) noexcept { return pi * (Float(1) / Float(180)) * deg; }

	class Point;
	class Vector;
	class DirectionVector;
	class Matrix;

	class Vector
	{
		public:
			Float x{}, y{};

			Vector(Float x, Float y);
			Vector() = default;

			Vector(const Point& begin, const Point& end);

			void operator*=(Float s);
			void operator/=(Float s);

			void operator+=(const Vector& other);
			void operator-=(const Vector& other);

			Vector perp()const;
			Vector rotate(Float theta)const;

			DirectionVector normalize()const;

			Point as_point()const;
	};

	Vector operator*(const Vector& vector, Float scalar);
	Vector operator/(const Vector& vector, Float scalar);
	Vector operator*(Float scalar, const Vector& vector);

	Vector operator+(const Vector& p, const Vector& q);
	Vector operator-(const Vector& p, const Vector& q);

	Vector operator-(const Vector& v);
	
	Float dot(const Vector& p, const Vector& q);
	
	Float det(const Vector& p, const Vector& q);

	Float length2(const Vector& v);
	Float length(const Vector& v);
	
	Vector perp(const Vector& v);
	Vector rotate(const Vector& v, Float theta);

	class Point
	{
		public:
			Float x{}, y{};
			Point(Float x, Float y);
			Point() = default;


			void operator+=(const Vector&);
			void operator-=(const Vector&);

			Point operator+(const Vector&)const;
			Point operator-(const Vector&)const;

			Vector as_vector()const;
	};

	Float distance2(const Point&p1, const Point&p2);
	Float distance(const Point& p1, const Point& p2);

	class DirectionVector
	{
		public:
			explicit DirectionVector(Float theta);
			explicit DirectionVector(const Vector&);
			DirectionVector() = delete;

			operator Vector()const;
			DirectionVector operator-()const;

			DirectionVector perp()const;

			Point as_point()const;
		private:
			DirectionVector(Float x, Float y);
			Float x, y;
	};

	Float length(const DirectionVector&);

	class Matrix
	{
		public:
			Float a{}, b{}, c{}, d{};
			//| a b |
			//| c d |
			Matrix(Float a, Float b, Float c, Float d);
			Matrix(const Vector& column_1, const Vector& column_2);
			Matrix() = default;

			static Matrix id();
			static Matrix clockwise_rotation(Float theta);
			static Matrix counterclockwise_rotation(Float theta);
			static Matrix stretching_x(Float k);
			static Matrix stretching_y(Float k);
			static Matrix scaling(Float k);
			static Matrix shearing_x(Float k);
			static Matrix shearing_y(Float k);
			static Matrix reflection(const Vector&);
			static Matrix orthogonal_projection(const Vector&);
			static Matrix change_basis(const Vector& new_basis_1, const Vector& new_basis_2);
			

			Matrix operator*(const Matrix& other)const;
			Matrix operator+(const Matrix& other)const;
			Matrix operator-(const Matrix& other)const;

			Vector operator*(const Vector& vector)const;

			Matrix operator*(Float scalar)const;
			Matrix operator/(Float scalar)const;

			Matrix operator-()const;

			Matrix adjugate()const;
			Matrix transpose()const;
			Float det()const;
	};

	//Vector world_to_screen(Vector v, const Vector& screen_origin, const Vector& screen_basis_x, const Vector& screen_basis_y);

	//Point change_basis(Point p, const Vector& new_basis_1, const Vector& new_basis_2);


	Point orthogonal_projection(const Point beg, const Point end, const Point p);
	Point reflection(const Point beg, const Point end, const Point p);

	Float distance2(const Point& beg, const Point& end, const Point& p);
	Float distance(const Point& beg, const Point& end, const Point& p);

	std::vector<Point> lines_intersection(const Point& beg_1, const Point& end_1, const Point& beg_2, const Point& end_2);
	
	class Line 
	{
		public:
			Point point;
			DirectionVector direction;

			Line(const Point& p1, const Point& p2);
			Line(const Point& p, const Vector& v);


			Float get_x_for_y(Float y)const;
			Float get_y_for_x(Float x)const;

			Point operator()(Float)const;

			Point reflection(const Point&)const;
			Point projection(const Point&)const;

			bool contains(const Point&)const;
	};
	std::vector<Point> lines_intersection(const Line& line_1, const Line& line_2);
	
	class LineSegment
	{
	public:
		Point beg;
		Point end;

		LineSegment(const Point& beg, const Point& end);

		Point closest_point(const Point& p)const;
		bool contains(const Point& p)const;
	};

	std::vector<Point> line_segments_intersection(const LineSegment& seg_1, const LineSegment& seg_2);


	class Circle
	{
	public:
		Point center;
		Float radius;

		explicit Circle(Point center = Point(Float(0), Float(0)), Float radius = Float(1));

		bool contains(const Point&)const;
	};


	class Stadium
	{
	public:
		Point beg;
		Point end;
		Float radius;

		Stadium(const Point& beg, const Point& end, Float radius);

		bool contains(const Point&)const;

		Circle closest_circle(const Point&)const;

		Vector normal()const;
	};
}