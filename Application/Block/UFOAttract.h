#pragma once
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collider/Sphere/Sphere.h"

class UFO;

class UFOAttract
{
public:
	void Initialize();
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
		if (std::holds_alternative<Screw*>(pairObject)) {
			screwCount_++;
		}
	};
	void SetCenter(const Vector3& center) { collider_->center_ = center; };
	void SetRadius(float r) { radius_ = r; };
	void Update() { collider_->worldTransformUpdate(); };//描画用
	Sphere* GetCollider() { return collider_.get(); };
	Vector3 GetCenter() { return collider_->center_; };
	float GetRadius() { return radius_; };
	void SetParent(UFO* ufo) { parent_ = ufo; };
	UFO* GetParent() { return parent_; };
	void SetScrewCount(int c) { screwCount_ = c; };
	int GetScrewCount() {return screwCount_ ; };
	void SetIsAttract(bool is) { isAttract_ = is; };
	bool GetIsAttract() {return isAttract_; };
private:
	std::unique_ptr<Sphere> collider_;
	float radius_ = 12.0f;
	UFO* parent_;
	int screwCount_;
	//WorldTransform* worldTransform_ = nullptr;
	bool isAttract_;
};
