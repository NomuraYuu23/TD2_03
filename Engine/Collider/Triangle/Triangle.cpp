#include "Triangle.h"

void Triangle::Initialize(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3)
{

	Collider::Initialize();

	SetVertices(vertices1, vertices2, vertices3);

}

void Triangle::SetVertices(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3)
{

	vertices_[0] = vertices1;
	vertices_[1] = vertices2;
	vertices_[2] = vertices3;

}

void Triangle::worldTransformUpdate()
{
}
