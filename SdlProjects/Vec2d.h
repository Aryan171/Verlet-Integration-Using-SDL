#pragma once
#include "Vec3d.h"

class Vec2d {
public:
	float x, y;

	Vec2d(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vec2d() {
		this->x = 0.0f;
		this->y = 0.0f;
	}
};

float Length(const Vec2d& a) {
	return pow((a.x * a.x) + (a.y * a.y), 0.5);
}

Vec2d operator +(const Vec2d& a,  const Vec2d& b) {
	return 	Vec2d(a.x + b.x, a.y + b.y);
}

Vec2d operator -(const Vec2d& a, const Vec2d& b) {
	return 	Vec2d(a.x - b.x, a.y - b.y);
}

Vec2d operator -(const Vec2d& a) {
	return Vec2d(-a.x, -a.y);
}

float Dot(const Vec2d& a, const Vec2d& b) {
	return (a.x * b.x) + (a.y * b.y);
}

Vec3d Cross(const Vec2d& a, const Vec2d& b) {
	return Vec3d(0.0f, 0.0f, (a.x * b.y) - (a.y * b.x));
}

Vec2d operator *(const Vec2d& a, float b) {
	return Vec2d(a.x * b, a.y * b);
}

Vec2d operator *(float a, const Vec2d& b) {
	return Vec2d(a * b.x, a * b.y);
}

Vec2d operator /(const Vec2d& a, float b) {
	return Vec2d(a.x / b, a.y / b);
}

Vec2d Normalize(const Vec2d& a) {
	return a / Length(a);
}

bool operator ==(const Vec2d& a, const Vec2d& b) {
	if (a.x == b.x && a.y == b.y) {
		return true;
	}
	else {
		return false;
	}
}

bool operator !=(const Vec2d& a, const Vec2d& b) {
	if (a.x != b.x || a.y != b.y) {
		return true;
	}
	else {
		return false;
	}
}

void operator +=(Vec2d& a, const Vec2d& b) {
	a.x += b.x;
	a.y += b.y;
}

void operator -=(Vec2d& a, const Vec2d& b) {
	a.x -= b.x;
	a.y -= b.y;
}

float Distance(const Vec2d& a, const Vec2d& b) {
	return pow(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)), 0.5);
}