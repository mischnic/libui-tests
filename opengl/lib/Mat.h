#ifndef MAT_H
#define MAT_H

#include "glew.h"
#include "Vec.h"
#include <stdio.h>

class Mat3 {
public:
	GLfloat m11, m12, m13;
	GLfloat m21, m22, m23;
	GLfloat m31, m32, m33;

	Mat3();

	Mat3(GLfloat m11, GLfloat m12, GLfloat m13,
		 GLfloat m21, GLfloat m22, GLfloat m23,
		 GLfloat m31, GLfloat m32, GLfloat m33);

	Mat3 transpose() const;
	Mat3 inverse() const;
	Vec3 transformNormal(Vec3 v) const;

	Mat3 operator*(Mat3 v) const;
	Vec3 operator*(Vec3 v) const;
};

std::ostream& operator<<(std::ostream &s, const Mat3 &v);

class Mat4 {
public:
	GLfloat m11, m12, m13, m14;
	GLfloat m21, m22, m23, m24;
	GLfloat m31, m32, m33, m34;
	GLfloat m41, m42, m43, m44;

	Mat4();

	Mat4(GLfloat m11, GLfloat m12, GLfloat m13, GLfloat m14,
		 GLfloat m21, GLfloat m22, GLfloat m23, GLfloat m24,
		 GLfloat m31, GLfloat m32, GLfloat m33, GLfloat m34,
		 GLfloat m41, GLfloat m42, GLfloat m43, GLfloat m44);

	void print() const;

	static Mat4 rotate(GLfloat theta, GLfloat x, GLfloat y, GLfloat z);
	static Mat4 perspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar);
	static Mat4 translate(GLfloat x, GLfloat y, GLfloat z);
	static Mat4 scale(GLfloat x, GLfloat y, GLfloat z);

	Mat4 operator*(Mat4 a) const;
	Vec3 operator*(Vec3 v) const;
	operator Mat3 () const;
	Mat3 to3() const;
};

std::ostream& operator<<(std::ostream &s, const Mat4 &v);

#endif
