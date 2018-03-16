#include "Vector3f.h"
#define _USE_MATH_DEFINES
#include <math.h>


Vector3f::Vector3f()
{
	m_x = m_y =	m_z = 0;
}

Vector3f::Vector3f(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

Vector3f::~Vector3f()
{
}

void Vector3f::Set(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

float Vector3f::GetX() const
{
	return m_x;
}

float Vector3f::GetY() const
{
	return m_y;
}

float Vector3f::GetZ() const
{
	return m_z;
}
Vector3f Vector3f::add(const Vector3f &vec) const
{
	return Vector3f(m_x + vec.GetX(), m_y + vec.GetY(), m_z + vec.GetZ());
}

Vector3f Vector3f::subtract(const Vector3f &vec) const
{
	return Vector3f(m_x - vec.GetX(), m_y - vec.GetY(), m_z - vec.GetZ());
}

Vector3f Vector3f::mult(float n) const
{
	return Vector3f(m_x*n, m_y*n, m_z*n);
}

Vector3f Vector3f::divide(float n) const
{
	return Vector3f(m_x / n, m_y / n, m_z/n);
}

float Vector3f::dot(const Vector3f &vec) const
{
	return m_x*vec.GetX() + m_y*vec.GetY() + m_z*vec.GetZ();
}

Vector3f Vector3f::cross(const Vector3f &vec) const
{
	return Vector3f(m_y*vec.GetZ() - m_z*vec.GetY(), m_x*vec.GetZ() - m_z*vec.GetX(), m_x*vec.GetY() - m_y*vec.GetX());
}

float Vector3f::length() const
{
	return static_cast<float>(sqrt(m_x*m_x + m_y*m_y + m_z*m_z));
}

float Vector3f::distance(const Vector3f &vec) const
{
	return subtract(vec).length();
}

Vector3f Vector3f::normalise() const
{
	float len = length();
	return Vector3f(m_x / len, m_y / len, m_z / len);
}


Vector3f operator+ (const Vector3f &lhs, const Vector3f &rhs)
{
	return lhs.add(rhs);
}

Vector3f operator- (const Vector3f &lhs, const Vector3f &rhs)
{
	return lhs.subtract(rhs);
}

Vector3f operator* (const Vector3f &lhs, float n)
{
	return lhs.mult(n);
}

Vector3f operator* (float n, const Vector3f &rhs)
{
	return rhs.mult(n);
}

Vector3f operator/ (const Vector3f &lhs, float n)
{
	return lhs.divide(n);
}

Vector3f& Vector3f::operator+= (const Vector3f &rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	m_z += rhs.m_z;
	return *this;
}

std::ostream& operator<< (std::ostream& out, const Vector3f v)
{
	return out << "(" << v.GetX() << "," << v.GetY() << "," << v.GetZ() << ")";
}