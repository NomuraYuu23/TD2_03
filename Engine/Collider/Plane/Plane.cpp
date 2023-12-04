#include "Plane.h"

void Plane::Initialize(const Vector3& normal, float distance)
{

	Collider::Initialize();

	normal_ = normal;//法線
	distance_ = distance;//距離

}

void Plane::worldTransformUpdate()
{
}
