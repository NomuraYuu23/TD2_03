#pragma once
#include "../Engine/3D/WorldTransform.h"
#include "../Engine/Collision/CollisionData.h"
#include "Collider/ColliderParentObject.h"
#include "../Engine/Collider/Sphere/Sphere.h"

class Magnet
{
public:
	void Initialize();
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {};
	void SetCenter(const Vector3& center) { collider_->center_ = center; };
	void SetRadius(float r) { radius_ = r; };
	void Update() { collider_->radius_ = radius_; collider_->worldTransformUpdate(); };//描画用
	Sphere* GetCollider() { return collider_.get(); };
	Vector3 GetCenter() { return collider_->center_; };
	float GetRadius() { return radius_; };
private:
	std::unique_ptr<Sphere> collider_;
	float radius_ = 12.0f;
	//WorldTransform* worldTransform_ = nullptr;
};
