#include "Shape.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
#include "Game.h"
#include "Sphere.h"

int Shape::countID = 0;

Shape::Shape(void)
{
}

Shape::Shape(float mass) : m_mass(mass)
{
}

Shape::~Shape(void)
{
}

void Shape::SetColor(int r, int g, int b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void Shape::SetPos(float x, float y, float z)
{
	m_pos.Set(x, y, z);
}

void Shape::SetNewPos(Vector3f pos)
{
	m_newPos = pos;
}

void Shape::SetVel(float x, float y, float z)
{
	m_velocity.Set(x, y, z);
}

void Shape::SetNewVel(Vector3f vel)
{
	m_newVelocity = vel;
}

void Shape::SetNewVelY(float f)
{
	m_newVelocity.Set(m_newVelocity.GetX(), f, m_newVelocity.GetZ());
}

void Shape::SetMass(float mass)
{
	m_mass = mass;
}

void Shape::SetFriction(const float& kf)
{
	Cf = kf;
	if(Cf < 0.0f)
		Cf = 0.0f;
}

void Shape::SetSpin(float rpm, Vector3f axis)
{
	float radps = rpm * 2.f * 3.14159f / 60.f;
	m_angVel = radps * axis.normalise();
}

void Shape::SetColliding(bool b)
{
	isColliding = b;
}

float Shape::GetFriction() const
{
	return Cf;
}

int Shape::GetID() const
{
	return m_objectID;
}

void Shape::CalculatePhysics(float dt)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	m_force.Set(0,0,0);

	const Vector3f gravity(0.0f, -9.81f * m_mass, 0.0f);

	m_force += gravity;

	/*if (isSliding)
	{
		m_force += m_friction;
	}*/
	/*else
	{
		m_force += gravity;
	}	*/	

	CalcVelPos(dt);

	//Rotation
	/*Vector3f angAccel = m_momentOfInertia.inverse()* m_torque;*/

	Matrix3d w_star = Matrix3d(0.0f, -m_newAngVel.GetZ(), m_newAngVel.GetY(),
		m_newAngVel.GetZ(), 0.0f, -m_newAngVel.GetX(),
		-m_newAngVel.GetY(), m_newAngVel.GetX(), 0.0f);

	m_newRot = exp(w_star*dt).multiply(m_rotation);
}

void Shape::CalcVelPos(float dt)
{
	Vector3f accel = m_force / m_mass;

	// Integrate accel to get the new velocity (using Euler)
	m_newVelocity = m_velocity + (accel * dt);



	Vector3f k1 = m_velocity + (accel * 0);
	Vector3f k2 = m_velocity + (accel + 0.5*k1) * 0.5 * dt;
	Vector3f k3 = m_velocity + (accel + 0.5*k2) * 0.5 * dt;
	Vector3f k4 = m_velocity + (accel + k3) * dt;

	m_newPos = m_pos + (1 / 6.0f) * (k1 + 2 * k2 + 2 * k3 + k4) *dt;

	 //Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	//Improved Euler
	//m_newPos = m_pos + 0.5 * ((m_velocity + m_newVelocity) * dt);
}

void Shape::SetFrictionForce(Vector3f v)
{
	m_friction = v;
}

void Shape::SetTorque(Vector3f v)
{
	m_torque = v;
}

void Shape::ResetPos()
{
	if(!isSliding)
		m_newPos = m_pos;
	else
		m_newPos = Vector3f(m_newPos.GetX(), m_pos.GetY(), m_newPos.GetZ());
}

void Shape::Update()
{
	m_velocity = m_newVelocity;
	m_pos = m_newPos;
	m_rotation = m_newRot;
}

float Shape::GetMass() const
{
	return m_mass;
}

Vector3f Shape::GetPos() const
{
	return m_pos;
}

Vector3f Shape::GetNewPos() const
{
	return m_newPos;
}

Vector3f Shape::GetVel() const
{
	return m_velocity;
}

Vector3f Shape::GetNewVel() const
{
	return m_newVelocity;
}
