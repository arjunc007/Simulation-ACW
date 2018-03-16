#pragma once
#include "Shape.h"

class Sphere;
struct Triangle
{
	Vector3f p0;
	Vector3f p1;
	Vector3f p2;
	Vector3f normal;
};

class Bowl : public Shape
{
public:
	Bowl();
	~Bowl();

	bool IsColliding(Sphere* sphere);

	void Render() const;

private:
	Vector3f GetPointOnSphere(int theta, int phi);

private:
	float r;
	Triangle triangles[1000];
	int numTriangles;
};

