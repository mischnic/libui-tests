#include "Mat.h"
#include <cmath>
#include <iostream>

Mat3::Mat3() :
	m11(1), m12(0), m13(0),
	m21(0), m22(1), m23(0),
	m31(0), m32(0), m33(1) {}

Mat3::Mat3(GLfloat m11, GLfloat m12, GLfloat m13,
		   GLfloat m21, GLfloat m22, GLfloat m23,
		   GLfloat m31, GLfloat m32, GLfloat m33) : 
		   m11(m11), m12(m12), m13(m13),
		   m21(m21), m22(m22), m23(m23),
		   m31(m31), m32(m32), m33(m33) {}

Mat3 Mat3::inverse() const {
	Mat3 result;
	Mat3 temp(*this);

	// Gauß-Jordan

	// get 1 at m11
	// 1. /= m11
	temp.m12 /= temp.m11;
	temp.m13 /= temp.m11;
	result.m11 /= temp.m11;
	result.m12 /= temp.m11;
	result.m13 /= temp.m11;
	temp.m11 = 1.0f;

	// get 0 at m21
	// 2. -= m21 * 1.
	temp.m22 -= temp.m12 * temp.m21;
	temp.m23 -= temp.m13 * temp.m21;
	result.m21 -= result.m11 * temp.m21;
	result.m22 -= result.m12 * temp.m21;
	result.m23 -= result.m13 * temp.m21;
	temp.m21 -= temp.m11 * temp.m21;

	// get 0 at m31
	// 3. -= m31 * 1.
	temp.m32 -= temp.m12 * temp.m31;
	temp.m33 -= temp.m13 * temp.m31;
	result.m31 -= result.m11 * temp.m31;
	result.m32 -= result.m12 * temp.m31;
	result.m33 -= result.m13 * temp.m31;
	temp.m31 -= temp.m11 * temp.m31;

	// get 1 at m22
	// 2. /= m22
	temp.m21 /= temp.m22;
	temp.m23 /= temp.m22;
	result.m21 /= temp.m22;
	result.m22 /= temp.m22;
	result.m23 /= temp.m22;
	temp.m22 = 1.0f;

	// get 0 at m32
	// 3. -= m32 * 2.
	temp.m31 -= temp.m21 * temp.m32;
	temp.m33 -= temp.m23 * temp.m32;
	result.m31 -= result.m21 * temp.m32;
	result.m32 -= result.m22 * temp.m32;
	result.m33 -= result.m23 * temp.m32;
	temp.m32 = 0.0f;
	temp.m32 -= temp.m22 * temp.m32;

	// get 1 at m33
	// 3. /= m33
	result.m31 /= temp.m33;
	result.m32 /= temp.m33;
	result.m33 /= temp.m33;
	temp.m33 = 1.0f;

	// get 0 at m23
	// 2. -= m23 * 3.
	result.m21 -= result.m31 * temp.m23;
	result.m22 -= result.m32 * temp.m23;
	result.m23 -= result.m33 * temp.m23;
	temp.m21 -= temp.m31 * temp.m23;
	temp.m22 -= temp.m32 * temp.m23;
	temp.m23 -= temp.m33 * temp.m23;
	
	// get 0 at m13
	// 1. -= m13 * 3.
	result.m11 -= result.m31 * temp.m13;
	result.m12 -= result.m32 * temp.m13;
	result.m13 -= result.m33 * temp.m13;
	temp.m11 -= temp.m31 * temp.m13;
	temp.m12 -= temp.m32 * temp.m13;
	temp.m13 -= temp.m33 * temp.m13;

	// get 0 at m12
	// 1. -= m12 * 2.
	result.m11 -= result.m21 * temp.m12;
	result.m12 -= result.m22 * temp.m12;
	result.m13 -= result.m23 * temp.m12;
	temp.m11 -= temp.m21 * temp.m12;
	temp.m12 -= temp.m22 * temp.m12;
	temp.m13 -= temp.m23 * temp.m12;
	temp.m12 = 0.0f;

	return result;
}

Mat3 Mat3::transpose() const {
	return Mat3(
		this->m11, this->m21, this->m31,
		this->m12, this->m22, this->m32,
		this->m13, this->m23, this->m33
	);
}

Vec3 Mat3::operator*(Vec3 v) const {
	return Vec3(
		this->m11 * v.x + this->m12 * v.y + this->m13 * v.z,
		this->m21 * v.x + this->m22 * v.y + this->m23 * v.z,
		this->m31 * v.x + this->m32 * v.y + this->m33 * v.z
	);
}

Vec3 Mat3::transformNormal(Vec3 v) const {
	return (*this).inverse().transpose() * v;
}

Mat3 Mat3::operator*(Mat3 b) const {
	return Mat3(
		this->m11 * b.m11 + this->m12 * b.m21 + this->m13 * b.m31,
		this->m11 * b.m12 + this->m12 * b.m22 + this->m13 * b.m32,
		this->m11 * b.m13 + this->m12 * b.m23 + this->m13 * b.m33,

		this->m21 * b.m11 + this->m22 * b.m21 + this->m23 * b.m31,
		this->m21 * b.m12 + this->m22 * b.m22 + this->m23 * b.m32,
		this->m21 * b.m13 + this->m22 * b.m23 + this->m23 * b.m33,

		this->m31 * b.m11 + this->m32 * b.m21 + this->m33 * b.m31,
		this->m31 * b.m12 + this->m32 * b.m22 + this->m33 * b.m32,
		this->m31 * b.m13 + this->m32 * b.m23 + this->m33 * b.m33
	);
}

std::ostream& operator<<(std::ostream &s, const Mat3 &v) {
	s << "Mat4(" << "\n" <<
	"    " << v.m11 << ", " << v.m12 << ", " << v.m13 << "\n" <<
	"    " << v.m21 << ", " << v.m22 << ", " << v.m23 << "\n" <<
	"    " << v.m31 << ", " << v.m32 << ", " << v.m33 << ")";
	return s;
}

Mat4::Mat4() :
	m11(1), m12(0), m13(0), m14(0),
	m21(0), m22(1), m23(0), m24(0),
	m31(0), m32(0), m33(1), m34(0),
	m41(0), m42(0), m43(0), m44(1) {}

Mat4::Mat4(GLfloat m11, GLfloat m12, GLfloat m13, GLfloat m14,
		   GLfloat m21, GLfloat m22, GLfloat m23, GLfloat m24,
		   GLfloat m31, GLfloat m32, GLfloat m33, GLfloat m34,
		   GLfloat m41, GLfloat m42, GLfloat m43, GLfloat m44) : 
		   m11(m11), m12(m12), m13(m13), m14(m14),
		   m21(m21), m22(m22), m23(m23), m24(m24),
		   m31(m31), m32(m32), m33(m33), m34(m34),
		   m41(m41), m42(m42), m43(m43), m44(m44) { Mat3(); }

void Mat4::print() const {
	printf("----\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n----\n",
		this->m11, this->m12, this->m13, this->m14,
		this->m21, this->m22, this->m23, this->m24,
		this->m31, this->m32, this->m33, this->m34,
		this->m41, this->m42, this->m43, this->m44
	);
}

Mat4 Mat4::perspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar)
{
	GLfloat f = 1.0f / tanf(fovy / 2.0f);
	Mat4 result(
		f / aspect, 0.0f, 0.0f                                 ,  0.0f,
		      0.0f,    f, 0.0f                                 ,  0.0f,
		      0.0f, 0.0f, (zfar + znear) / (znear - zfar)      , (2.0 * zfar * znear) / (znear - zfar),
		      0.0f, 0.0f, -1.0f,  0.0f
	);
	return result;
}

Mat4 Mat4::translate(GLfloat x, GLfloat y, GLfloat z)
{
	Mat4 result(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return result;
}

Mat4 Mat4::rotate(GLfloat theta, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat c = cosf(theta), s = sinf(theta);
	GLfloat ci = 1.0f - c;
	Mat4 result(
		x*x*ci + c  , x*y*ci - z*s, x*z*ci + y*s, 0.0,
		y*x*ci + z*s, y*y*ci + c  , y*z*ci - x*s, 0.0,
		x*z*ci - y*s, y*z*ci + x*s, z*z*ci + c  , 0.0,
		         0.0,          0.0,          0.0, 1.0
	);
	return result;
}

Mat4 Mat4::scale(GLfloat x, GLfloat y, GLfloat z) {
	Mat4 result(
		x, 0, 0, 0,		
		0, y, 0, 0,		
		0, 0, z, 0,		
		0, 0, 0, 1.0f	
	);
	return result;
}

Mat4 Mat4::operator*(Mat4 b) const {
	return Mat4(
		this->m11 * b.m11 + this->m12 * b.m21 + this->m13 * b.m31 + this->m14 * b.m41,
		this->m11 * b.m12 + this->m12 * b.m22 + this->m13 * b.m32 + this->m14 * b.m42,
		this->m11 * b.m13 + this->m12 * b.m23 + this->m13 * b.m33 + this->m14 * b.m43,
		this->m11 * b.m14 + this->m12 * b.m24 + this->m13 * b.m34 + this->m14 * b.m44,

		this->m21 * b.m11 + this->m22 * b.m21 + this->m23 * b.m31 + this->m24 * b.m41,
		this->m21 * b.m12 + this->m22 * b.m22 + this->m23 * b.m32 + this->m24 * b.m42,
		this->m21 * b.m13 + this->m22 * b.m23 + this->m23 * b.m33 + this->m24 * b.m43,
		this->m21 * b.m14 + this->m22 * b.m24 + this->m23 * b.m34 + this->m24 * b.m44,

		this->m31 * b.m11 + this->m32 * b.m21 + this->m33 * b.m31 + this->m34 * b.m41,
		this->m31 * b.m12 + this->m32 * b.m22 + this->m33 * b.m32 + this->m34 * b.m42,
		this->m31 * b.m13 + this->m32 * b.m23 + this->m33 * b.m33 + this->m34 * b.m43,
		this->m31 * b.m14 + this->m32 * b.m24 + this->m33 * b.m34 + this->m34 * b.m44,

		this->m41 * b.m11 + this->m42 * b.m21 + this->m43 * b.m31 + this->m44 * b.m41,
		this->m41 * b.m12 + this->m42 * b.m22 + this->m43 * b.m32 + this->m44 * b.m42,
		this->m41 * b.m13 + this->m42 * b.m23 + this->m43 * b.m33 + this->m44 * b.m43,
		this->m41 * b.m14 + this->m42 * b.m24 + this->m43 * b.m34 + this->m44 * b.m44
	);
}

Vec3 Mat4::operator*(Vec3 v) const{
	return Vec3(
		this->m11 * v.x + this->m12 * v.y + this->m13 * v.z + this->m14,
		this->m21 * v.x + this->m22 * v.y + this->m23 * v.z + this->m24,
		this->m31 * v.x + this->m32 * v.y + this->m33 * v.z + this->m34
	);
}

Mat4::operator Mat3 () const {
	return this->to3();
}

Mat3 Mat4::to3() const {
	return Mat3(
		m11, m12, m13,
		m21, m22, m23,
		m31, m32, m33
	);
}

std::ostream& operator<<(std::ostream &s, const Mat4 &v) {
	s << "Mat4(" << "\n" <<
	"    " << v.m11 << ", " << v.m12 << ", " << v.m13 << ", " << v.m14 << "\n" <<
	"    " << v.m21 << ", " << v.m22 << ", " << v.m23 << ", " << v.m24 << "\n" <<
	"    " << v.m31 << ", " << v.m32 << ", " << v.m33 << ", " << v.m34 << "\n" <<
	"    " << v.m41 << ", " << v.m42 << ", " << v.m43 << ", " << v.m44 << ")";
	return s;
}