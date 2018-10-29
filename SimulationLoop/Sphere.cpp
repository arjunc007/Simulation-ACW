#include "Sphere.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
#include "Game.h"
#include "Plane.h"
#include "Bowl.h"
#include <fstream>
#include<assert.h>

const float zero = 1e-4f;

Sphere::Sphere(void) : Shape(0.02f), m_radius(0.005f)
{
	m_objectID = countID++;
	m_texture = TextureLoader::LoadBMP("checker.bmp");
	SetColor(1, 0, 0);
	isColliding = false;
	float I = (2.f * m_mass * m_radius * m_radius) / 3.0f;
	m_momentOfInertia = Matrix3d(I, 0, 0, 0, I, 0, 0, 0, I);
	m_rotation = Matrix3d::MatrixIdentity();
	vrot.Set(0, 0, 0);
}

Sphere::~Sphere(void)
{
}

int Sphere::IsColliding(Sphere* sphere) const
{
	Vector3f dist = this->GetNewPos() - sphere->GetNewPos();

	float distance = dist.length() - this->GetRadius()-sphere->GetRadius();

	if (distance < 0.0f)
	{
		return 1;
	}
	else if(distance > zero)
		return -1;
	else 
		return 0;
}

void Sphere::CollisionDetection(Plane* plane, ContactManifold* contactManifold, const float &dt)
{
	if (plane->IsColliding(this))
	{
		Vector3f colNormal = (plane->GetNormal().dot(m_pos - plane->GetPos()) * plane->GetNormal()).normalise();

		//CalcTimeOfImpact(dt, plane, colNormal);

		contactManifold->Add({ this, plane, colNormal, dt });
		isColliding = true;
	}
}

void Sphere::CalcTimeOfImpact(const float & dt, Plane * plane, Vector3f &colNormal)
{
	//Calculate actual time of impact
	double t = dt * 0.5f, t_start = 0, t_end = dt;
	double dn = 0;
	//std::ofstream out("test.txt", std::ofstream::out | std::ofstream::app);
	//out << "loop start\n";
	while (true)
	{
		CalcVelPos(t);
		dn = (m_newPos - plane->GetPos()).dot(colNormal) - m_radius;

		if (fabs(dn) < 10e-4 && dn < 0)
			int x = 0;

		if (dn < 0.0)
		{
			t_end = t;
			t = (t_start + t) * 0.5;
		}
		else if (dn > 10e-4)
		{
			t_start = t;
			t = (t + t_end) * 0.5;
		}
		else break;

		assert(t > 0 && t < dt);
	}
}

void Sphere::CollisionDetection(Sphere* sphere2, ContactManifold *contactManifold, float dt)
{
	if (sphere2->IsColliding(this) == 1) {
		Vector3f dist = this->GetNewPos() - sphere2->GetNewPos();
		Vector3f colNormal = dist.normalise();
		contactManifold->Add({ this, sphere2, colNormal, dt });
		isColliding = true;
		sphere2->SetColliding(true);
	}
}

void Sphere::CollisionDetection(Bowl* bowl, ContactManifold *contactManifold, float dt)
{
	if (bowl->IsColliding(this))
	{
		Vector3f normal = (bowl->GetPos() - m_pos).normalise();
		contactManifold->Add({ this, bowl, normal, dt });
		isColliding = true;
	}
}

void Sphere::CollisionResponse(ManifoldPoint &point)
{
	ResetPos();
	
	Vector3f colNormal = point.contactNormal;

	Plane* wallPtr = dynamic_cast<Plane*>(point.contactID2);

	Vector3f Vrel = point.contactID1->GetVel() - point.contactID2->GetVel();

	if (wallPtr != nullptr)
	{
		//2nd object is a plane
		if (Vrel.GetY() < 10e-4 && !m_isGrounded)
		{
			m_isGrounded = true;
			//Apply friction
			m_newVelocity.Set(m_newVelocity.GetX(), 0, m_newVelocity.GetZ());
		}
	}

	//For collision with wall, add impactforce one after the other to the zeroed one.
	float t = point.dt;

	Vector3f colPoint = (point.contactID1->GetNewPos() + m_radius*colNormal);

	Vector3f ra = point.contactID1->GetNewPos() - colPoint;
	Vector3f rb = point.contactID2->GetNewPos() - colPoint;
	float m_inv = 1 / (point.contactID1->GetMass()) + 1 / (point.contactID2->GetMass());
	float Ia_inv = (m_momentOfInertia.inverse()*ra.cross(colNormal)).cross(ra).dot(colNormal);
	float Ib_inv = (m_momentOfInertia.inverse()*(rb.cross(colNormal))).cross(rb).dot(colNormal);
	float J = max(-(1 + Game::e)*Vrel.dot(colNormal) / (m_inv + Ia_inv + Ib_inv), 0);

	m_impulse += J*colNormal;

	point.contactID2->AddImpulse(-J * colNormal);

	//Friction	

	/*Vector3f V_t = Vrel - colNormal.dot(Vrel)*colNormal;
	Vector3f colTangent = V_t.normalise();

	float J_t = max(V_t.dot(colTangent) / (m_inv + Ia_inv + Ib_inv), 0);*/

	//m_impactforce += -0.3f*J_t*colTangent;
	/*m_impactforce += -point.contactID2->GetFriction()*J_t*colTangent/t;
	
	m_torque = ra.cross(m_impactforce.length()*colNormal);

	point.contactID2->SetImpactForce(-1.f*m_impactforce);
	point.contactID2->SetTorque(rb.cross(-m_impactforce.length()*colNormal));*/
}

float Sphere::GetRadius() const
{
	return m_radius;
}

void Sphere::Render() const									
{
	float rotation[16] = {m_rotation.GetRow(0).GetX(), m_rotation.GetRow(0).GetY(), m_rotation.GetRow(0).GetZ(), 0,
		m_rotation.GetRow(1).GetX(), m_rotation.GetRow(1).GetY(), m_rotation.GetRow(1).GetZ(), 0,
		m_rotation.GetRow(2).GetX(), m_rotation.GetRow(2).GetY(), m_rotation.GetRow(2).GetZ(), 0,
		0, 0, 0, 1};

	Vector3f axis = m_angVel.normalise();

	glPushMatrix();
		glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ());
		//glRotatef(vrot.length() * 200, axis.GetX(), axis.GetY(), axis.GetZ());
		glMultMatrixf(rotation);
		glColor3d(color.r, color.g, color.b);
		glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
		GLUquadric *quadric = gluNewQuadric();
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluSphere(quadric, m_radius, 20, 20);
		gluDeleteQuadric(quadric);
	glPopMatrix();
}
