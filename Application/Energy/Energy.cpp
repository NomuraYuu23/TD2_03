#include "Energy.h"
#include "../Block/Block.h"
void Energy::Initialize() {
	worldTransform_.Initialize();

	mat_.reset(Material::Create());
	TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	mat_->Update(t, { 0.0f,1.0f,0.0f,0.5f }, 0, 200);
	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);
	innerAreaCount_=0;
}

void Energy::Update() {
	worldTransform_.transform_.translate = target_;
	float halfSize = (target_.z - startPoint_.z) / 2.0f;
	worldTransform_.transform_.translate.z = halfSize + startPoint_.z;
	worldTransform_.transform_.scale = {10.0f,0.1f,6.0f};
	worldTransform_.transform_.scale.z = halfSize;
	worldTransform_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->size_ = worldTransform_.transform_.scale;
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	innerAreaCount_ = 0;
}

void Energy::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_, camera, mat_.get());
}

void Energy::SetTarget(const Vector3& position) {
	target_ = position;
	startPoint_ = target_;
	startPoint_.z = -200.0f;
}

void Energy::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (std::holds_alternative<Block*>(pairObject)) {
		innerAreaCount_++;
	}
}