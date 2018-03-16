#pragma once

#include "Vector3f.h"
#include "Shape.h"
#include "Hole.h"

class Sphere;
class Plane : public Shape
{
public:
	Plane();
	Plane(Vector3f &normal, Vector3f &v1, Vector3f &v2, const float &length);
	~Plane();

	Vector3f GetPoint() const;
	Vector3f GetNormal() const;

	int IsColliding(Sphere* sphere) const override;
	bool IsMoving() const;
	void SetMoving(bool);
	void AddHoles();

	void StartMoving(float dir, float t);
	void UpdatePos(float dt);
	void Reset();

	void Render() const;
	void CollisionDetection(Shape* shape, ContactManifold* manifold) {}

private:
	Vector3f m_normal, t, b;
	Vector3f vertices[4];
	float m_diag, m_length, m_width;
	bool moving;
	std::vector<Hole> holes;
};

