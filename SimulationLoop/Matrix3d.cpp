#include "Matrix3d.h"
#include <assert.h>



Matrix3d::Matrix3d()
{
	r[0] = Vector3f();
	r[1] = Vector3f();
	r[2] = Vector3f();
}

Matrix3d::Matrix3d(Vector3f a, Vector3f b, Vector3f c)
{
	r[0] = a;
	r[1] = b;
	r[2] = c;
}

Matrix3d::Matrix3d(float m_0, float m_1, float m_2, float m_3, float m_4, float m_5, float m_6, float m_7, float m_8)
{
	r[0] = Vector3f(m_0, m_1, m_2);
	r[1] = Vector3f(m_3, m_4, m_5);
	r[2] = Vector3f(m_6, m_7, m_8);
}


Matrix3d::~Matrix3d()
{
}

Vector3f Matrix3d::GetRow(int i) const
{
	return r[i];
}

Vector3f Matrix3d::GetCol(int i) const
{
	return this->transpose().GetRow(i);
}

float Matrix3d::GetElement(int i, int j) const
{
	if (j == 0)
		return r[i].GetX();
	if (j == 1)
		return r[i].GetY();
	if (j == 2)
		return r[i].GetZ();
	return NULL;
}

Matrix3d Matrix3d::MatrixIdentity()
{
	return Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

Matrix3d Matrix3d::transpose() const
{
	Vector3f a = Vector3f(r[0].GetX(), r[1].GetX(), r[2].GetX());
	Vector3f b = Vector3f(r[0].GetY(), r[1].GetY(), r[2].GetY());
	Vector3f c = Vector3f(r[0].GetZ(), r[1].GetZ(), r[2].GetZ());
	return Matrix3d(a, b, c);
}

Matrix3d Matrix3d::add(const Matrix3d &m) const
{
	Vector3f a = r[0] + m.GetRow(0);
	Vector3f b = r[1] + m.GetRow(1);
	Vector3f c = r[2] + m.GetRow(2);
	return Matrix3d(a, b, c);
}
Matrix3d Matrix3d::subtract(const Matrix3d &m) const
{
	Vector3f a = r[0] - m.GetRow(0);
	Vector3f b = r[1] - m.GetRow(1);
	Vector3f c = r[2] - m.GetRow(2);
	return Matrix3d(a, b, c);
}
Matrix3d Matrix3d::multiply(const Matrix3d &m) const
{
	Matrix3d t = m.transpose();

	Matrix3d output(r[0].dot(t.GetRow(0)), r[0].dot(t.GetRow(1)), r[0].dot(t.GetRow(2))
		, r[1].dot(t.GetRow(0)), r[1].dot(t.GetRow(1)), r[1].dot(t.GetRow(2))
		, r[2].dot(t.GetRow(0)), r[2].dot(t.GetRow(1)), r[2].dot(t.GetRow(2)));
	return output;
}

Vector3f Matrix3d::multiply(const Vector3f &v) const
{
	return Vector3f(r[0].dot(v), r[1].dot(v), r[2].dot(v));
}
Matrix3d Matrix3d::multiply(const float &f) const
{
	return Matrix3d(f*r[0], f*r[1], f*r[2]);
}

Matrix3d Matrix3d::divide(const float &f) const
{
	return Matrix3d(r[0] / f, r[1] / f, r[2] / f);
}

Matrix3d pow(const Matrix3d &m, const int &p)
{
	assert(p >= 0);
	if (p == 0)
		return Matrix3d::MatrixIdentity();
	else
	{
		Matrix3d out = m;
		for (int i = 1; i < p; i++)
		{
			out = out.multiply(m);
		}
		return out;
	}
}

Matrix3d sigma(const Matrix3d &m, const int &n)
{
	return pow(Matrix3d::MatrixIdentity() + m.multiply(1.f / n), n);
}

int fact(int i)
{
	if (i == 0 || i==1)
		return 1;
	else
		return fact(i - 1)*i;
}

Matrix3d exp(const Matrix3d &m)
{
	const int kmax = 4;
	Matrix3d Y[kmax][kmax];
	for (int i = 0; i < kmax; i++)
		Y[i][0] = sigma(m, pow(2,i));
	for (int k = 1; k < kmax; k++)
		for (int i = 0; i < kmax - k; i++)
			Y[i][k] = Y[i + 1][k - 1] + (Y[i + 1][k - 1] - Y[i][k - 1])*(1.f / (pow(2, k) - 1));
	return Y[0][kmax - 1];
}

float Matrix3d::det() const
{
	float d = r[0].GetX()*(r[1].GetY()*r[2].GetZ() - r[1].GetZ()*r[2].GetY())
		- r[0].GetY()*(r[1].GetX()*r[2].GetZ() - r[1].GetZ()*r[2].GetX())
		+ r[0].GetZ()*(r[1].GetX()*r[2].GetY() - r[1].GetY()*r[2].GetX());
	return d;
}

Matrix3d Matrix3d::inverse() const
{
	Matrix3d m;
	if (this->det() > 0.0000000001f)
	{
		Vector3f a = Vector3f(r[0].GetX()*(r[1].GetY()*r[2].GetZ() - r[1].GetZ()*r[2].GetY())
		, -r[0].GetY()*(r[1].GetX()*r[2].GetZ() - r[1].GetZ()*r[2].GetX())
		, r[0].GetZ()*(r[1].GetX()*r[2].GetY() - r[1].GetY()*r[2].GetX()));
		Vector3f b = Vector3f(-r[0].GetX()*(r[1].GetY()*r[2].GetZ() - r[1].GetZ()*r[2].GetY())
			, r[0].GetY()*(r[1].GetX()*r[2].GetZ() - r[1].GetZ()*r[2].GetX())
			, -r[0].GetZ()*(r[1].GetX()*r[2].GetY() - r[1].GetZ()*r[2].GetX()));
		Vector3f c = Vector3f(r[0].GetX()*(r[1].GetY()*r[2].GetZ() - r[1].GetZ()*r[2].GetY())
			, -r[0].GetY()*(r[1].GetX()*r[2].GetZ() - r[1].GetZ()*r[2].GetX())
			, r[0].GetZ()*(r[1].GetX()*r[2].GetY() - r[1].GetY()*r[2].GetX()));
		m = Matrix3d(a, b, c);
	}
	return m / this->det();
}

Matrix3d operator+ (const Matrix3d &lhs, const Matrix3d &rhs)
{
	return lhs.add(rhs);
}

Matrix3d operator- (const Matrix3d &lhs, const Matrix3d &rhs)
{
	return lhs.subtract(rhs);
}

Matrix3d operator/(const Matrix3d &lhs, const float &rhs)
{
	return lhs.divide(rhs);
}

Matrix3d operator* (const Matrix3d &lhs, const float &rhs)
{
	return lhs.multiply(rhs);
}

Vector3f operator* (const Matrix3d &lhs, const Vector3f &rhs)
{
	return lhs.multiply(rhs);
}