#include "Vector2f.h"

Vector2f::Vector2f() :
	x(0),
	y(0)
{
}

Vector2f::Vector2f(float _x, float _y) :
	x(_x),
	y(_y)
{
}

Vector2f::~Vector2f()
{
}

void Vector2f::operator=(const Vector2f & v)
{
	x = v.x;
	y = v.y;
}

bool Vector2f::operator==(const Vector2f & v)
{
	return (x == v.x && y == v.y) ? true : false;
}

bool Vector2f::operator!=(const Vector2f & v)
{
	return (x == v.x && y == v.y) ? false : true;
}

Vector2f Vector2f::operator/(const float val)
{
	return Vector2f(x / val, y / val);
}

Vector2f Vector2f::operator+(const Vector2f & v)
{
	return Vector2f(x + v.x, y + v.y);
}

Vector2f Vector2f::operator+(const float & val)
{
	return Vector2f(x + val, y + val);
}

void Vector2f::operator+=(const Vector2f & v)
{
	//Add values to our x and y
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(const Vector2f & v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2f::operator/=(const Vector2f & v)
{
	x /= v.x;
	y /= v.y;
}

void Vector2f::operator/=(const float & val)
{
	x /= val;
	y /= val;
}

void Vector2f::operator*=(float val)
{
	x *= val;
	y *= val;
}

Vector2f Vector2f::operator-(const Vector2f & v)
{
	return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::operator*(float val)
{
	return Vector2f(x * val, y * val);
}

Vector2f Vector2f::operator*(Vector2f v)
{
	return Vector2f(x * v.x, y * v.y);
}

std::ostream & operator<<(std::ostream & out, const Vector2f & v)
{
	out << "(" << std::to_string(v.x);
	out << ", " << std::to_string(v.y) << ")";
	return out;
}

float Vector2f::distance(Vector2f & v1, Vector2f & v2)
{
	auto minus = v1 - v2;
	return minus.magnitude();
}

float Vector2f::distance(Vector2f & v2)
{
	auto minus = *this - v2;
	return minus.magnitude();
}

float Vector2f::magnitude()
{
	return sqrtf((x * x) + (y * y));;
}

float Vector2f::sqrdMagnitude()
{
	return (x * x) + (y * y);
}

float Vector2f::dotProduct(Vector2f v)
{
	return (x * v.x) + (y * v.y);
}

float Vector2f::crossProduct(Vector2f v)
{
	return (x * v.y) - (y * v.x);
}

float Vector2f::angle()
{
	return toDeg(atan2f(y, x));
}

Vector2f Vector2f::crossProduct(float s)
{
	return Vector2f(s * y, -s * x);
}

Vector2f Vector2f::normalise()
{
	if (x == 0 && y == 0)
		return Vector2f(0,0);

	float len = this->magnitude();
	return Vector2f(x / len, y / len);
}

Vector2f Vector2f::rotate(float degrees)
{
	float radians = toRad(degrees);
	float s = sin(radians);
	float c = cos(radians);

	float tx = x;
	float ty = y;

	return Vector2f(c * tx - s * ty, s * tx + c * ty);
}

Vector2f Vector2f::vectorFromAngle(float degrees)
{
	float rad = toRad(degrees);
	return Vector2f(cos(rad), sin(rad));
}

void Vector2f::zeroVector()
{
	x = 0;
	y = 0;
}

float Vector2f::toRad(float deg)
{
	return deg * (M_PI / 180);
}

float Vector2f::toDeg(float rad)
{
	return rad * (180 / M_PI);
}
