#pragma once

#include "Shape.h"

class Bowl;
class Plane;
class Sphere : public Shape
{
public:
	Sphere(void);
	~Sphere(void);

	int IsColliding(Sphere* sphere) const;
	void CollisionDetection(Plane* plane, ContactManifold *contactManifold, const float &dt);
	void CollisionDetection(Sphere* sphere, ContactManifold *contactManifold, float dt);
	void CollisionDetection(Bowl* bowl, ContactManifold *contactManifold, float dt);
	void CollisionResponse(ManifoldPoint &point);
	//void CollisionResponse(Plane* plane);
	//void CollisionResponseWithSphere(ManifoldPoint &point);
	//void CollisionResponseWithPlane(ManifoldPoint &point) {}

	float GetRadius() const;

	void Render() const;

private:
	float m_radius;
};

