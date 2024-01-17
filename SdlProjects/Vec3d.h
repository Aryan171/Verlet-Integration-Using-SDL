#pragma once
#include <corecrt_math.h>

class Vec3d {
public:
	float x, y, z;

	Vec3d(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3d() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}
};

float Length(const Vec3d& a) {
	return pow((a.x * a.x) + (a.y * a.y) + (a.z * a.z), 0.5);
}

Vec3d operator +(const Vec3d& a, const Vec3d& b) {
	return 	Vec3d(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3d operator -(const Vec3d& a, const Vec3d& b) {
	return 	Vec3d(a.x - b.x, a.y - b.y, a.z - a.z);
}

Vec3d operator -(const Vec3d& a) {
	return Vec3d(-a.x, -a.y, -a.z);
}

float Dot(const Vec3d& a, const Vec3d& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vec3d Cross(const Vec3d& a, const Vec3d& b) {
	return Vec3d((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

Vec3d operator *(const Vec3d& a, float b) {
	return Vec3d(a.x * b, a.y * b, a.z * b);
}

Vec3d operator *(float a, const Vec3d& b) {
	return Vec3d(b.x * a, b.y * a, b.z * a);
}

Vec3d operator /(const Vec3d& a, float b) {
	return Vec3d(a.x / b, a.y / b, a.z / b);
}

Vec3d Normalize(const Vec3d& a) {
	return a / Length(a);
}

bool operator ==(const Vec3d& a, const Vec3d& b) {
	if (a.x == b.x && a.y == b.y && a.z == b.z) {
		return true;
	}
	else {
		return false;
	}
}

bool operator !=(const Vec3d& a, const Vec3d& b) {
	if (a.x != b.x || a.y != b.y || a.z != b.z) {
		return true;
	}
	else {
		return false;
	}
}

void operator +=(Vec3d& a, const Vec3d& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

void operator -=(Vec3d& a, const Vec3d& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}

float Distance(const Vec3d a, const Vec3d b) {
	return pow(((a.y - b.y) * (a.y - b.y)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)), 0.5);
}