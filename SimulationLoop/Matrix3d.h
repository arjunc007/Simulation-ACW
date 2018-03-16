#pragma once

#include"Vector3f.h"

class Matrix3d
{
public:
	Matrix3d();
	Matrix3d(Vector3f a, Vector3f b, Vector3f c);
	Matrix3d(float m_0, float m_1, float m_2, float m_3, float m_4, float m_5, float m_6, float m_7, float m_8);
	~Matrix3d();

	Vector3f GetRow(int i) const;
	Vector3f GetCol(int i) const;
	float GetElement(int i, int j) const;

	static Matrix3d MatrixIdentity();
	Matrix3d transpose() const;
	Matrix3d add(const Matrix3d &m) const;
	Matrix3d subtract(const Matrix3d &m) const;
	Matrix3d multiply(const Matrix3d &m) const;
	Vector3f multiply(const Vector3f &v) const;
	Matrix3d multiply(const float &f) const;
	Matrix3d divide(const float &f) const;
	float det() const;
	Matrix3d inverse() const;

private:
	Vector3f r[3];
};

Matrix3d operator+ (const Matrix3d &lhs, const Matrix3d &rhs);
Matrix3d operator- (const Matrix3d &lhs, const Matrix3d &rhs);
Matrix3d operator/ (const Matrix3d &lhs, const float &rhs);
Matrix3d operator* (const Matrix3d &lhs, const float &rhs);
Vector3f operator* (const Matrix3d &lhs, const Vector3f &rhs);
Matrix3d pow(const Matrix3d &m, const int &p);
Matrix3d exp(const Matrix3d&m);
int fact(int);

