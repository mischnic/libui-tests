#ifndef VEC_H
#define VEC_H

#include "glew.h"
#include <cmath>
#include <iostream>

class Vec2
{
public:
	GLfloat x, y;

	Vec2();
	Vec2(GLfloat x, GLfloat y);
};

class Vec3
{
public:
	GLfloat x, y, z;

	Vec3();
	Vec3(GLfloat x, GLfloat y, GLfloat z);

	GLfloat length() const;
	Vec3 normalize() const;

	GLfloat operator*(Vec3 v) const;

	Vec3 operator+(Vec3 v) const;
	Vec3 operator-(Vec3 v) const;
	Vec3 operator*(GLfloat v) const;
	Vec3 operator/(GLfloat v) const;
	Vec3 operator-() const;

};

std::ostream& operator<<(std::ostream &s, const Vec3 &v);

#endif