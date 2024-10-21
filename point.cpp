#include "point.h"
#include "body.h"

using namespace RBX;

Point::Point(Body* _body)
	:numOwners(1),
	body(_body)
	{}

Point::Point(Body& _body)
	:numOwners(1)
	//body(_body)
	{}

void Point::step()
{
	RBXAssert(force); //placeholder
}

void Point::forceToBody()
{
	RBXAssert(force); //placeholder
}

void Point::setWorldPos(const G3D::Vector3& argWorldPos)
{
	worldPos = argWorldPos;
}

/*inline bool Point::sameBodyAndOffset(const RBX::Point& p1, const RBX::Point& p2)
{
	return (p1.body == p2.body
		&& p1.localPos.x == p2.localPos.x
		&& p1.localPos.y == p2.localPos.y
		&& p1.localPos.z == p2.localPos.z);
}*/