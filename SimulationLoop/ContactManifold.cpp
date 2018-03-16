#include "ContactManifold.h"
#include "Sphere.h"
#include <assert.h>

ContactManifold::ContactManifold(void)
{
}


ContactManifold::~ContactManifold(void)
{
}

void ContactManifold::Add(ManifoldPoint point)
{
	m_points.push_back(point);
}

void ContactManifold::RemovePoint(int i)
{
	m_points.erase(m_points.begin() + i);
}

void ContactManifold::Clear()
{
	while (GetNumPoints())
		m_points.pop_back();
}

int ContactManifold::GetNumPoints() const
{
	return m_points.size();
}

ManifoldPoint& ContactManifold::GetPoint(int i)
{
	return m_points.at(i);
}

bool ContactManifold::CheckIfColliding(Shape* shape) const
{
	for (auto point : m_points)
	{
		if (point.contactID1 == shape || point.contactID2 == shape)
			return true;
	}
	return false;
}

bool ContactManifold::CheckDuplicate(Shape* shape, int end) const
{
	for (int i = 0; i < end; i++)
	{
		ManifoldPoint point = m_points.at(i);
		if (point.contactID1 == shape || point.contactID2 == shape)
			return true;
	}
	return false;
}