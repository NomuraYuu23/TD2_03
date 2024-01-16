#include "UFO.h"
#include "../Screw/Screw.h"
void UFO::Initialize() {
	Block::Initialize();
	isDead_ = false;
	worldTransform_.transform_.translate.y = 12.0f;
	worldTransform_.UpdateMatrix();
	colliderUFO_.reset(new OBB);
	colliderUFO_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);

	attract_.reset(new UFOAttract);
	attract_->Initialize();
	attract_->SetParent(this);
	magnet_.reset(new Magnet);
	magnet_->Initialize();
	TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	materialCircle_.reset(Material::Create());
	//t.scale = {magnet_->GetRadius(),1.0f,magnet_->GetRadius() };
	materialCircle_->Update(t, { 0.8f,0.8f,0.0f,0.5f }, 0, 200);
	worldTransformCircle_.Initialize();
	worldTransformCircle_.transform_.scale = { attract_->GetRadius(),0.5f,attract_->GetRadius() };
	theta_ = -3.14f/2.0f;
	screwCount_ = 0;
}

void UFO::Update() {
	isAttract_ = false;
	if (!isDead_) {
		isAttract_ = true;
		if (attract_->GetScrewCount() < 2) {
			worldTransform_.transform_.translate.x = std::sinf(theta_) * 20.0f;
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity_);
			theta_ += 0.01f;
			isAttract_ = false;
		}worldTransform_.UpdateMatrix();
		colliderUFO_->center_ = worldTransform_.GetWorldPosition();
		colliderUFO_->SetOtientatuons(worldTransform_.rotateMatrix_);
		Vector3 attractPos = worldTransform_.GetWorldPosition();
		attractPos.y -= attract_->GetRadius();
		attract_->SetCenter(attractPos);
		worldTransformCircle_.transform_.translate = attractPos;
		worldTransformCircle_.UpdateMatrix();
		
		attract_->SetScrewCount(0);
		attract_->SetIsAttract(isAttract_);
	}
	else if (isDead_) {
		materialCircle_->SetColor({ 0.8f,0.8f,0.8f,0.5f });
		Block::Update();
		worldTransformCircle_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransformCircle_.UpdateMatrix();
		magnet_->SetCenter(worldTransformCircle_.GetWorldPosition());
	}
	screwCount_ = 0;
}

void UFO::Draw(Model* model, BaseCamera& camera) {
	Block::Draw(model, camera);
	//if (!isDead_) {
		modelCircle_->Draw(worldTransformCircle_, camera, materialCircle_.get());
	//}
	
}

void UFO::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (std::holds_alternative<Screw*>(pairObject)) {
		if (std::get<Screw*>(pairObject)->GetState() == Screw::REVERSE) {
			isDead_ = true;
			worldTransform_.transform_.translate.y = 0;
		}
		else {
			std::get<Screw*>(pairObject)->SetIsDead(true);
		}
	}
}
