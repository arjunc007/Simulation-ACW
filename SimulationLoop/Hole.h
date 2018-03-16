#pragma once
#include "Shape.h"

class Hole : public Shape
{
public:
	Hole();
	~Hole();

	float GetRadius() const;

	void Render() const;

private:
	float m_radius;
};

