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
	const float I = (2.f * m_mass * m_radius * m_radius) / 3.0f;
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
	const float dist = plane->GetNormal().dot(m_pos - plane->GetPos());
	const float penetration = m_radius - dist;

	if (penetration > 0)
	{
		Vector3f d = m_pos - plane->GetPos();
		if (fabsf(d.dot(plane->GetTangent())) < plane->GetWidth() / 2.0f &&
			fabsf(d.dot(plane->GetBinormal())) < plane->GetLength() / 2.0f)
		{
			contactManifold->Add({ this, plane, plane->GetNormal(), dt, penetration });
		}
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

void Sphere::CollisionDetection(Sphere* other, ContactManifold *contactManifold, float dt)
{
	Vector3f relPos = m_pos - other->GetPos();
	float dist = relPos.length();
	float minSourceDist = m_radius + other->GetRadius();

	if (dist < minSourceDist)
	{
		float penetration = minSourceDist - dist;
		Vector3f normal = relPos.normalise();

		contactManifold->Add({ this, other, normal, dt, penetration });
	}
}

void Sphere::CollisionDetection(Bowl* bowl, ContactManifold *contactManifold, float dt)
{
	float penetration;
	Vector3f normal;

	if (bowl->IsColliding(this, penetration, normal))
	{
		contactManifold->Add({ this, bowl, normal, dt, penetration });
	}
}

void Sphere::CollisionResponse(ManifoldPoint &point)
{	
	Shape* A = point.contactID1;
	Shape* B = point.contactID2;
	Vector3f n = point.contactNormal;

	Vector3f ra = n * -m_radius;

	Vector3f rb;
	Sphere* sphereB = dynamic_cast<Sphere*>(B);
	if (sphereB)
	{
		rb = n * sphereB->GetRadius();
	}
	else
	{
		rb = Vector3f();
	}

	Vector3f va = A->GetVel() + A->GetAngVel().cross(ra);
	Vector3f vb = B->GetVel() + B->GetAngVel().cross(rb);

	Vector3f vRel = va - vb;

	float contactVel = vRel.dot(n);

	if (contactVel > 0)
	{
		//Objects already moving apart
		return;
	}

	float invMassA = 1.f / A->GetMass();
	float invMassB = (B->GetMass() > 1000000.f) ? 0.f : 1.f / B->GetMass();
	float rotA = (A->GetInertiaInv() * ra.cross(n)).cross(ra).dot(n);
	float rotB = 0.f; 
	if (sphereB)
	{
		rotB = (sphereB->GetInertiaInv() * rb.cross(n)).cross(rb).dot(n);
	}

	float J = -(1.f + Game::e) * contactVel;
	J /= (invMassA + invMassB + rotA + rotB);

	Vector3f impulse = J * n;

	A->AddImpulse(impulse);
	if (invMassB > 0)
	{
		B->AddImpulse(impulse * -1.f);
	}

	A->AddAngularImpulse(ra.cross(impulse));
	if (sphereB)
	{
		sphereB->AddAngularImpulse(rb.cross(impulse * -1.f));
	}

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
