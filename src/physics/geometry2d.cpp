#include "geometry2d.h"


namespace gm2d
{
	Vector::Vector(Float x, Float y)
		: x{ x }, y{ y }
	{}

	Vector::Vector(const Point& begin, const Point& end)
		: x{ end.x - begin.x }, y{ end.y - begin.y }
	{}

	void Vector::operator*=(Float s) {
		x *= s;
		y *= s;
	}

	void Vector::operator/=(Float s) {
		x /= s;
		y /= s;
	}

	void Vector::operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
	}

	void Vector::operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
	}

	Point Vector::as_point()const {
		return Point(x, y);
	}

	Vector Vector::perp()const {
		return gm2d::perp(*this);
	}
	Vector Vector::rotate(Float theta)const {
		return gm2d::rotate(*this, theta);
	}

	DirectionVector Vector::normalize()const {
		return DirectionVector(*this);
	}

	Vector operator*(const Vector& vector, Float scalar) {
		return Vector(scalar * vector.x, scalar * vector.y);
	}

	Vector operator/(const Vector& vector, Float scalar) {
		return Vector(vector.x / scalar, vector.y / scalar);
	}

	Vector operator*(Float scalar, const Vector& vector) {
		return Vector(vector.x * scalar, vector.y * scalar);
	}

	Vector operator+(const Vector& p, const Vector& q) {
		return Vector(p.x + q.x, p.y + q.y);
	}

	Vector operator-(const Vector& p, const Vector& q) {
		return Vector(p.x - q.x, p.y - q.y);
	}

	Vector operator-(const Vector& v) {
		return Vector(-v.x, -v.y);
	}

	Float dot(const Vector& p, const Vector& q) {
		return p.x * q.x + p.y * q.y;
	}

	Float det(const Vector& p, const Vector& q) {
		return p.x * q.y - q.x * p.y;
	}

	Float length2(const Vector& v) {
		return v.x * v.x + v.y * v.y;
	}

	
	Float length(const Vector& v) {
		return std::sqrt(length2(v));
	}

	
	Vector perp(const Vector& v) {
		return Vector(-v.y, v.x);
	}

	Vector rotate(const Vector& v, Float theta) {
		const Float nx = v.x * std::cos(theta) - v.y * std::sin(theta);
		const Float ny = v.y * std::cos(theta) + v.x * std::sin(theta);
		return Vector(nx, ny);
	}

	Point::Point(Float x, Float y)
		: x{ x }, y{ y }
	{}

	void Point::operator+=(const Vector&v) {
		x += v.x;
		y += v.y;
	}
	void Point::operator-=(const Vector& v) {
		x -= v.x;
		y -= v.y;
	}

	Vector Point::as_vector()const {
		return Vector(x, y);
	}

	Point Point::operator+(const Vector& v)const {
		return Point(x + v.x, y + v.y);
	}

	Point Point::operator-(const Vector& v)const {
		return Point(x - v.x, y - v.y);
	}

	Float distance2(const Point& p1, const Point& p2) {
		return length2({ p1, p2 });
	}

	Float distance(const Point& p1, const Point& p2) {
		return length({ p1, p2 });
	}

	DirectionVector::DirectionVector(Float theta)
		: x{ std::cos(theta) }, y{ std::sin(theta) }
	{}

	DirectionVector::DirectionVector(const Vector& v)
		: x{ v.x / length(v) }, y{ v.y / length(v) }
	{}

	DirectionVector::DirectionVector(Float x, Float y)
		: x{ x }, y{ y }
	{}

	DirectionVector::operator Vector()const {
		return Vector(x, y);
	}

	DirectionVector DirectionVector::operator-()const {
		return DirectionVector(-x, -y);
	}

	Float length(const DirectionVector& v) {
		return Float(1);
	}


	Matrix::Matrix(Float a, Float b, Float c, Float d)
		: a{ a }, b{ b }, c{ c }, d{ d }
	{}

	Matrix Matrix::id() {
		return Matrix(
			Float(1), Float(0),
			Float(0), Float(1));
	}
	Matrix Matrix::clockwise_rotation(Float theta) {
		return Matrix(
			std::cos(theta), std::sin(theta),
			-std::sin(theta), std::cos(theta));
	}
	Matrix Matrix::counterclockwise_rotation(Float theta) {
		return Matrix(
			std::cos(theta), -std::sin(theta),
			std::sin(theta), std::cos(theta));
	}

	Matrix Matrix::stretching_x(Float k) {
		return Matrix(
			Float(k), Float(0),
			Float(0), Float(1));
	}

	Matrix Matrix::stretching_y(Float k) {
		return Matrix(
			Float(1), Float(0),
			Float(0), Float(k));
	}

	Matrix Matrix::scaling(Float k) {
		return Matrix(
			Float(k), Float(0),
			Float(0), Float(k));
	}

	Matrix Matrix::shearing_x(Float k) {
		return Matrix(
			Float(1), Float(k),
			Float(0), Float(1));
	}

	Matrix Matrix::shearing_y(Float k) {
		return Matrix(
			Float(1), Float(0),
			Float(k), Float(1));
	}

	Matrix Matrix::reflection(const Vector& v) {
		const Float d = Float(1) / dot(v, v);
		return Matrix(
			(v.x * v.x - v.y * v.y) * d, Float(2) * v.x * v.y * d,
			Float(2) * v.x * v.y * d, (v.y * v.y - v.x * v.x) * d);
	}

	Matrix Matrix::orthogonal_projection(const Vector& u) {
		const Float d = 1.f / dot(u, u);
		return Matrix(
			u.x * u.x * d, u.x * u.y * d,
			u.x * u.y * d, u.y * u.y * d);
	}
	
	Matrix Matrix::operator*(Float s)const {
		return Matrix(a * s, b * s, c * s, d * s);
	}

	Matrix Matrix::operator/(Float s)const {
		return Matrix(a / s, b / s, c / s, d / s);
	}

	Matrix Matrix::operator-()const {
		return Matrix(-a, -b, -c, -d);
	}

	Vector Matrix::operator*(const Vector& v)const {
		return Vector(v.x * a + v.y * b, v.x * c + v.y * d);
	}

	Matrix Matrix::operator*(const Matrix& m)const {
		return Matrix(
			m.a * a + m.c * b,
			m.b * a + m.d * b,
			m.a * c + m.c * d,
			m.b * c + m.d * d);
	}

	Matrix Matrix::operator+(const Matrix& other)const {
		return Matrix(a + other.a, b + other.b, c + other.c, d + other.d);
	}

	Matrix Matrix::operator-(const Matrix& other)const {
		return Matrix(a - other.a, b - other.b, c - other.c, d - other.d);
	}

	Matrix Matrix::transpose()const {
		return Matrix(a, c, b, d);
	}

	Float Matrix::det()const {
		return a * d - b * c;
	}

	Matrix Matrix::adjugate() const {
		return Matrix(d, -b, -c, a);
	}


	Point orthogonal_projection(const Point beg, const Point end, const Point p) {
		const Vector v(beg, end);
		return (Matrix::orthogonal_projection(v) * (p - beg.as_vector()).as_vector() + beg.as_vector()).as_point();
	}

	Point reflection(const Point beg, const Point end, const Point p) {
		const Vector v(beg, end);
		return (Matrix::reflection(v) * (p - beg.as_vector()).as_vector() + beg.as_vector()).as_point();
	}

	Float distance2(const Point& beg, const Point& end, const Point& p) {
		return distance2(orthogonal_projection(beg, end, p), p);
	}

	Float distance(const Point& beg, const Point& end, const Point& p) {
		return std::sqrt(distance2(beg, end, p));
	}

	std::vector<Point> lines_intersection(const Point& beg_1, const Point& end_1, const Point& beg_2, const Point& end_2) {

		const Vector direction_1(beg_1, end_1);
		const Vector direction_2(beg_2, end_2);

		const Float Det = det(direction_1, -direction_2);

		const Float Det_1 = det(Vector(beg_1, beg_2), -direction_2);
		const Float Det_2 = det(direction_1, Vector(beg_1, beg_2));

		std::vector<Point>points{};


		if (is_nearly_zero(Det)) {
			if (is_nearly_zero(Det_1) or is_nearly_zero(Det_2)) {
				points.push_back(beg_1);
				points.push_back(beg_2);
			}
		}
		else {
			const Float t = Det_1 / Det;
			points.push_back(beg_1 + t * direction_1);
		}

		return points;
	}

	////
	Line::Line(const Point& p1, const Point& p2)
		: point{ p1 }, direction(Vector(p1, p2))
	{}

	Line::Line(const Point& p, const Vector& v)
		: point{ p }, direction(v)
	{}

	Point Line::operator()(Float t)const {
		return point + direction * t;
	}

	Point Line::reflection(const Point& p)const {
		return (point.as_vector() + Matrix::reflection(direction) * (p - point.as_vector()).as_vector()).as_point();
	}

	Point Line::projection(const Point& p)const {
		return (point.as_vector() + Matrix::orthogonal_projection(direction) * (p - point.as_vector()).as_vector()).as_point();
	}


	Float Line::get_x_for_y(Float y)const {
		return (*this)((y - point.y) / ((Vector)direction).y).x;
	}

	Float Line::get_y_for_x(Float x)const {
		return (*this)((x - point.x) / ((Vector)direction).x).y;
	}

	std::vector<Point> lines_intersection(const Line& line_1, const Line& line_2) {
		return lines_intersection(line_1.point, line_1(Float(1)), line_2.point, line_2(Float(1)));
	}

	Circle::Circle(Point center, Float r)
		: center{ center }, radius{ r }
	{}

	bool Circle::contains(const Point& p)const {
		return distance2(center, p) <= radius * radius;
	}
}