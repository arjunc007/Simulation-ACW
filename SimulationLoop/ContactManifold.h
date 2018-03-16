#pragma once

#include "Vector3f.h"
#include<vector>

class Shape;

struct ManifoldPoint {
	Shape *contactID1;
	Shape *contactID2;
	Vector3f contactNormal;
	float dt;
};

class ContactManifold
{
public:
	ContactManifold(void);
	~ContactManifold(void);

	void Add(ManifoldPoint point);
	void RemovePoint(int i);
	void Clear();
	int GetNumPoints() const;
	ManifoldPoint& GetPoint(int i);
	bool CheckIfColliding(Shape*) const;
	bool CheckDuplicate(Shape*, int) const;

private:
	// ******************************************************************************************
	// The max number of points in the manifold is not verified in the code, so you need to test!
	// Maybe it would be safer to use a dyamic data sturcture (although this may be too slow)?
	// ******************************************************************************************
	std::vector<ManifoldPoint> m_points;
};

