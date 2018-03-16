#include "Bowl.h"
#include "Sphere.h"
#include <cmath>
#define _USE_MATH_DEFINES



Bowl::Bowl()
{
	m_objectID = countID++;

	int i = 0;
	r = 0.2f;
	int dtheta = 10;								//Change in degrees per point on latitude
	int dphi = 5;									//Change in degrees per point on longitude
	int thetamax = 360;
	int phimax = 45;
	for (int phi = 0; phi < 45; phi += dphi)		//41.41deg for 5cm
		for (int theta = 0; theta < 360; theta += dtheta)
		{
			int thetan = (theta + dtheta == thetamax) ? thetamax : theta + dtheta;
			int phin = (phi + dphi == phimax) ? phimax : phi + phimax;

			Vector3f p0 = GetPointOnSphere(theta, phi);
			Vector3f p1 = GetPointOnSphere(theta, phin);
			Vector3f p2 = GetPointOnSphere(thetan, phi);
			Vector3f p3 = GetPointOnSphere(thetan, phin);
			triangles[i++] = {p0, p2, p1, (p0-p2).cross(p2-p1).normalise()};
			triangles[i++] = { p3, p1, p2, (p3 - p1).cross(p1 - p2).normalise() };
		}
	numTriangles = i;
	m_pos = Vector3f(0.0f, 0.075f, 0.0f);
}


Bowl::~Bowl()
{
}

bool Bowl::IsColliding(Sphere* sphere)
{
	float r = sphere->GetRadius();
	Vector3f sPos = sphere->GetNewPos();
	Vector3f sVel = sphere->GetVel();
	if (sPos.GetY() < -0.075f + r)
	{
		float dist = (m_pos - sPos).length() - r;
		if (dist > 0.2f)
			return true;
	}
	return false;
}

Vector3f Bowl::GetPointOnSphere(int u, int v)
{
	const float deg2rad = static_cast<float>(3.14159 / 180);
	return Vector3f(r*sin(v*deg2rad)*cos(u*deg2rad),
		r*sin(v*deg2rad)*sin(u*deg2rad),
		r*cos(v*deg2rad));
	
}

void Bowl::Render() const
{
	glPushMatrix();
		//glTranslatef(0.0f, -0.125f, 0.0f);
		glRenderMode(GL_MODELVIEW);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLES);
		glColor4d(0.54, 0.27, 0.07, 0.5f);
		for (int i = 0; i < numTriangles; i++)
		{
			glVertex3d(triangles[i].p0.GetX(), triangles[i].p0.GetY(), triangles[i].p0.GetZ());
			glVertex3d(triangles[i].p1.GetX(), triangles[i].p1.GetY(), triangles[i].p1.GetZ());
			glVertex3d(triangles[i].p2.GetX(), triangles[i].p2.GetY(), triangles[i].p2.GetZ());
		}
		glEnd();
	glPopMatrix();
}
