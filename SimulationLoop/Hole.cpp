#include "Hole.h"
#include "TextureLoader.h"
#include <gl\gl.h>  
#include <gl\GLU.h>



Hole::Hole() : m_radius(0.01)
{
}


Hole::~Hole()
{
}

float Hole::GetRadius() const
{
	return m_radius;
}

void Hole::Render() const
{
	//Draw Planes
	glPushMatrix();
		//glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ());
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(m_pos.GetX(), m_pos.GetY() + 0.00001f, m_pos.GetZ());
		for (int i = 0; i <= 500; i++)
			glVertex3f(m_pos.GetX() + m_radius*sin(i*6.2831853 / 500), m_pos.GetY(), m_pos.GetZ() + m_radius*cos(i*6.2831853 / 500));
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(m_pos.GetX(), m_pos.GetY() - 0.00001f, m_pos.GetZ());
		for (int i = 0; i <= 500; i++)
			glVertex3f(m_pos.GetX() + m_radius*sin(i*6.2831853 / 500), m_pos.GetY(), m_pos.GetZ() + m_radius*cos(i*6.2831853 / 500));
		glEnd();
	glPopMatrix();

	//Draw Spheres
	/*glPushMatrix();
	glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ());
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, m_radius, 20, 20);
	gluDeleteQuadric(quadric);
	glPopMatrix();*/
}