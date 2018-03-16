#pragma once

#include "ContactManifold.h"
#include "Vector3f.h"
#include <Windows.h>
#include "Matrix3d.h"
#include <gl\gl.h>  

class Sphere;
struct ColorRGB {
	int r, g, b;
};

class Shape
{
public:
	Shape(void);
	~Shape(void);

	void CalculatePhysics(float dt);
	void CalcVelPos(float a);
	void Update();
	void SetColor(int r, int g, int b);
	void SetPos(float x, float y, float z);
	void SetVel(float x, float y, float z);
	void SetNewPos(Vector3f pos);
	void SetNewVel(Vector3f vel);
	void SetNewVelY(float f);
	void SetMass(float mass);
	void SetFriction(const float &kf);
	void SetSpin(float f, Vector3f axis);
	void SetColliding(bool b);

	Vector3f GetPos() const;
	Vector3f GetNewPos() const;
	Vector3f GetVel() const;
	Vector3f GetNewVel() const;
	float GetMass() const;
	float GetFriction() const;
	int GetID() const;
	void ResetPos();
	void SetFrictionForce(Vector3f);
	void SetTorque(Vector3f);

	virtual int IsColliding(Sphere* shape) const { return 5; }
	virtual void CollisionDetection(Shape* shape, ContactManifold* manifold, float dt) {}
	virtual void CollisionResponse(ManifoldPoint &point) {}
	virtual void CollisionResponseWithSphere(ManifoldPoint &point) {}
	virtual void CollisionResponseWithPlane(ManifoldPoint &point) {}
	virtual void Render() const = 0;

	static int countID;

protected:
	Shape(float m);

	ColorRGB color;
	Vector3f m_force;
	Vector3f m_friction;							//Frictional force
	float m_mass;
	Matrix3d m_momentOfInertia;
	Matrix3d m_rotation;
	Vector3f vrot;
	Matrix3d m_newRot;
	Vector3f m_angVel;
	Vector3f m_newAngVel;
	Vector3f m_torque;
	Vector3f m_pos;
	Vector3f m_newPos;
	Vector3f m_velocity;
	Vector3f m_newVelocity;
	float Cf = 0.f;									//Coefficient of friction
	bool isColliding = false;
	bool isSliding = false;
	int m_objectID;
	GLuint m_texture;
};