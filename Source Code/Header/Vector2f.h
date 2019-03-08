#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <math.h>

/**
* Description: Vector2f class, will be used since SDL doesnt have a vector class and this includes more functionality (dot product, cross product, normalise etc.)
*/
class Vector2f
{
public:
	Vector2f(); //Default constructor
	Vector2f(float _x, float _y); //Constructor that takes in 2 values (x and y)
	~Vector2f(); //Destructor

	//Operator overloading
	void operator=(const Vector2f &v); //Assignment operator
	bool operator==(const Vector2f &v);//Equals operator
	bool operator!=(const Vector2f &v);//Equals operator
	Vector2f operator/(const float val); //Division operator
	Vector2f operator+(const Vector2f &v); //Plus operator
	Vector2f operator+(const float &val); //Plus operator
	void operator+=(const Vector2f &v); //Plus equals operator
	void operator-=(const Vector2f &v); //Minus equals operator
	void operator/=(const Vector2f &v); //Division equals operator
	void operator/=(const float &val); //Division equals operator
	void operator*=(float val); //Multiplication equals operator
	Vector2f operator-(const Vector2f &v); //Minus operator
	Vector2f operator*(float val); //Multiplication operator
	Vector2f operator*(Vector2f v); //Multiplication operator
	friend std::ostream & operator << (std::ostream &out, const Vector2f &v); //Cout operator to easily debug vectors

	//Vector operations
	float distance(Vector2f& v1, Vector2f& v2);
	float distance(Vector2f& v2);
	float magnitude();
	float sqrdMagnitude();
	float dotProduct(Vector2f v);
	float crossProduct(Vector2f v);
	float angle();
	Vector2f crossProduct(float s);
	Vector2f normalise();
	Vector2f rotate(float degrees);
	Vector2f vectorFromAngle(float degrees);
	void zeroVector(); //Reset vector to 0,0

	//Public variables
	float x, y;

private:
	float toRad(float deg);
	float toDeg(float rad);
};
