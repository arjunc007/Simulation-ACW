#pragma once
#include<iostream>

class Vector3f
{
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();

	void Set(float x, float y, float z);
	float GetX() const;
	float GetY() const;
	float GetZ() const;

	Vector3f add(const Vector3f &vec) const;
	Vector3f subtract(const Vector3f &vec) const;
	Vector3f mult(float n) const;
	Vector3f divide(float n) const;
	float dot(const Vector3f &vec) const;
	Vector3f cross(const Vector3f &vec) const;
	float length() const;
	float distance(const Vector3f &vec) const;
	Vector3f normalise() const;

	Vector3f& operator+= (const Vector3f &rhs);

private:
	float m_x, m_y, m_z;
};

Vector3f operator+ (const Vector3f &lhs, const Vector3f &rhs);
Vector3f operator- (const Vector3f &lhs, const Vector3f &rhs);
Vector3f operator* (const Vector3f &lhs, float n);
Vector3f operator* (float n, const Vector3f &lhs);
Vector3f operator/ (const Vector3f &lhs, float n);
std::ostream& operator<< (std::ostream& out, const Vector3f v);