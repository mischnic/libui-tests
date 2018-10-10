#include "Vec.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(GLfloat x, GLfloat y) : x(x), y(y) {}

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

GLfloat Vec3::length() const {
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::normalize() const {
	GLfloat length = this->length();
	if(length == 0) return Vec3(*this);
	return *this / length;
}

Vec3 Vec3::operator+(Vec3 v) const {
	return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(Vec3 v) const {
	return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

GLfloat Vec3::operator*(Vec3 v) const {
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vec3 Vec3::operator*(GLfloat v) const {
	return Vec3(this->x * v, this->y * v, this->z * v);
}

Vec3 Vec3::operator/(GLfloat v) const {
	return Vec3(this->x / v, this->y / v, this->z / v);
}

Vec3 Vec3::operator-() const {
	return Vec3(-this->x, -this->y, -this->z);
}

std::ostream& operator<<(std::ostream &s, const Vec3 &v) {
	s << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return s;
}
