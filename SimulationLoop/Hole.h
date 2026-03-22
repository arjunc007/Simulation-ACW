#pragma once
#include "Shape.h"

class Hole : public Shape
{
public:
	Hole();
	~Hole() override;

	float GetRadius() const;

	void Render() const;

private:
	float m_radius;
};

