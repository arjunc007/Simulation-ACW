#include "Plane.h"
#include "Sphere.h"
#include "TextureLoader.h"

const float zero = 1e-4f;

Plane::Plane() : m_normal(0.0f, 1.0f, 0.0f)
{
}

Plane::Plane(Vector3f &normal, Vector3f &v1, Vector3f &v2, const float &length) : m_normal(normal)
{
	m_objectID = countID++;
	Vector3f d = v2 - v1;
	m_width = d.length();
	m_length = length;
	t = d.normalise();
	b = normal.cross(t);

	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v2 + b*length;
	vertices[3] = v1 + b*length;
	m_pos = (vertices[0] + vertices[1]) / 2.f + b*length / 2.f;
	m_newPos = Vector3f(1000000000.0f, 0.f, 0.f);
	m_mass = 1000000000;
	m_diag = (vertices[0] - vertices[2]).length();
	moving = false;
	m_texture = NULL;
}

void Plane::AddHoles()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			holes.push_back(Hole());
			holes.back().SetPos(-0.03f + i*0.03f, 0.0f, -0.03f + j*0.03f);
		}
}

Plane::~Plane()
{
}

bool Plane::IsMoving() const
{
	return moving;
}

void Plane::SetMoving(bool b)
{
	moving = b;
}

int Plane::IsColliding(Sphere* sphere) const
{
	float r = sphere->GetRadius();
	Vector3f newpos(sphere->GetNewPos());
	Vector3f pos(sphere->GetPos());

	float oldDist = m_normal.dot(pos - m_pos);
	float newDist = m_normal.dot(newpos - m_pos);

	float dtan = fabsf((pos - m_pos).dot(t));
	float dbinorm = fabsf((pos - m_pos).dot(b));

	//check within bounds of  plane
	if (dtan < r + m_width / 2.f && dbinorm < r + m_length / 2.f)
	{
		//Intersection if points on opposite sides
		if (oldDist * newDist < 0.f)
		{
			for (auto it = holes.begin(); it != holes.end(); it++)
			{
				if ((pos - it->GetPos()).length() < it->GetRadius())
					return 0;
			}
			return 1;
		}

		//If start or end inside radius
		if (fabsf(oldDist) <= r || fabsf(newDist) <= r)
		{
			for (auto it = holes.begin(); it != holes.end(); it++)
			{
				if ((pos - it->GetPos()).length() < it->GetRadius())
							return 0;
			}
			return 1;
		}
	}
	//No collision otherwise
	return 0;
}

Vector3f Plane::GetPoint() const
{
	return vertices[0];
}

Vector3f Plane::GetNormal() const
{
	return m_normal;
}

void Plane::StartMoving(float dirX, float t)
{
	moving = true;
	Vector3f movDir = Vector3f(dirX, 0.f, 0.f);
	
	m_velocity = (movDir*0.1f) / t;
}

void Plane::UpdatePos(float dt)
{
	if (moving)
	{
		Vector3f newPos;
		newPos = m_pos + m_velocity*dt;
		if (newPos.GetX() >= 0.1f || newPos.GetX() <= 0.0f)
		{
			newPos = m_pos;
			moving = false;
			return;
		}
		Vector3f velocity = m_velocity;
		Vector3f moveDist = (newPos-m_pos).length()*velocity.normalise();
		vertices[0] += moveDist;
		vertices[1] += moveDist;
		vertices[2] += moveDist;
		vertices[3] += moveDist;
		m_pos = newPos;
	}
}

void Plane::Reset()
{
	m_pos.Set(0.0f, m_pos.GetY(), m_pos.GetZ());
	m_velocity = Vector3f();
	vertices[0] = m_pos + t*m_width / 2.f + b*m_length/2.f;
	vertices[1] = m_pos - t*m_width / 2.f + b*m_length / 2.f;
	vertices[2] = m_pos - t*m_width / 2.f - b*m_length / 2.f;
	vertices[3] = m_pos + t*m_width / 2.f - b*m_length / 2.f;
}

void Plane::Render() const
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBegin(GL_QUADS);
	glColor4d(color.r, color.g, color.b, 0.3f);
	glVertex3d(vertices[0].GetX(), vertices[0].GetY(), vertices[0].GetZ());
	glVertex3d(vertices[1].GetX(), vertices[1].GetY(), vertices[1].GetZ());
	glVertex3d(vertices[2].GetX(), vertices[2].GetY(), vertices[2].GetZ());
	glVertex3d(vertices[3].GetX(), vertices[3].GetY(), vertices[3].GetZ());
	glEnd();

	for (auto it = holes.begin(); it != holes.end(); it++)
		it->Render();
}