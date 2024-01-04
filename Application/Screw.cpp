#include "Screw.h"
#include "Player.h"
#include "Block/Block.h"
#include "../Engine/Math/Ease.h"
void(Screw::* Screw::stateTable[])() = {
	&Screw::None,
	& Screw::Follow,
	& Screw::Reverse,
	& Screw::ToBlock,
	& Screw::Stuck,
	& Screw::ToPlayer
};

void Screw::Initialize() {
	state_ = State::FOLLOW;
	//state_ = State(5);
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = {0.8f,0.8f,0.8f};

	mat_.reset(Material::Create());
	TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	mat_->Update(t, { 0.0f,0.0f,0.8f,1.0f }, 0, 200);

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);

}
void Screw::Update() {
	(this->*stateTable[static_cast<size_t>(state_)])();
	worldTransform_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();
}
void Screw::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_, camera,mat_.get());
}

void Screw::Throw(const Vector3 position, void* block, size_t num) {
	state_ = TOBLOCK;
	startPosition_ = position;
	target_ = block;
	static_cast<Block*>(target_)->SetAnchorPointScrew(num,this);
	targetNum_ = num;
	frameCount_ = 0;
}

void Screw::Catch() {
	state_ = TOPLAYER;

	static_cast<Block*>(target_)->SetAnchorPointScrew(targetNum_, nullptr);

	startPosition_ = worldTransform_.GetWorldPosition();
	worldTransform_.parent_ = nullptr;
	target_ = player_;
	frameCount_ = 0;
}

void Screw::None() {
	worldTransform_.transform_.rotate.z = reverseT_ * 3.14f;
	reverseT_ =  float(frameCount_) / float(kReverseSpeed_);
	reverseT_ = std::clamp(reverseT_,0.0f,1.0f);
	float distance = Vector3Calc::Length(Vector3Calc::Subtract(worldTransform_.GetWorldPosition(), player_->GetWorldTransform()->GetWorldPosition()));
	if (reverseT_ <= 0.0f && distance <= player_->GetMagnet()->GetRadius()) {
		worldTransform_.transform_.rotate.z = 0;
		state_ = FOLLOW;
	}
	frameCount_--;

	worldTransform_.transform_.translate.y -= 0.3f;
	if (worldTransform_.transform_.translate.y <= -20.0f) {
		worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
		worldTransform_.transform_.translate.y = 4.0f;
	}
}

void Screw::Follow() {
	float distance = Vector3Calc::Length(Vector3Calc::Subtract(worldTransform_.GetWorldPosition(), player_->GetWorldTransform()->GetWorldPosition()));
	if (distance >= 2.0f) {
		Vector3 velocity =  Vector3Calc::Multiply(kFollowSpeed, Vector3Calc::Normalize(Vector3Calc::Subtract(player_->GetWorldTransform()->GetWorldPosition(), worldTransform_.transform_.translate)));
		velocity.y = 0;
		worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity);
	}
	//worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
	worldTransform_.transform_.translate.y -= 0.3f;
	if (worldTransform_.transform_.translate.y <= -20.0f) {
		worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
		worldTransform_.transform_.translate.y = 4.0f;
	}
}

void Screw::Reverse() {
	worldTransform_.transform_.rotate.z = reverseT_ * 3.14f;
	reverseT_ = float(frameCount_) / float(kReverseSpeed_);
	reverseT_ = std::clamp(reverseT_, 0.0f, 1.0f);
	
	if (frameCount_ >= kReverseTime) {
		state_ = NONE;
		reverseT_ = 1.0f;
		frameCount_ = kReverseSpeed_;
	}
	
	frameCount_++;

	worldTransform_.transform_.translate.y -= 0.3f;
	if (worldTransform_.transform_.translate.y <= -20.0f) {
		worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
		worldTransform_.transform_.translate.y = 4.0f;
	}
}

void Screw::ToBlock() {
	float t = float(frameCount_) / 30.0f;
	Vector3 endPoint = static_cast<Block*>(target_)->GetAnchorPointWorldPosition(targetNum_);
	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseInSine,startPosition_,endPoint,t);
	frameCount_++;
	if (frameCount_>30) {
		state_ = STUCK;
	}
}

void Screw::Stuck(){
	Vector3 endPoint = static_cast<Block*>(target_)->GetAnchorPointWorldPosition(targetNum_);
	worldTransform_.transform_.translate = endPoint;
}


void Screw::ToPlayer() {
	float t = float(frameCount_) / 30.0f;
	Vector3 endPoint = static_cast<Player*>(target_)->GetWorldTransform()->GetWorldPosition();
	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseInSine, startPosition_, endPoint, t);
	frameCount_++;
	if (frameCount_ > 30) {
		state_ = FOLLOW;
	}
}

void Screw::TurnOver() {
	if (state_ == FOLLOW || state_ == NONE) {
		state_ = REVERSE;
		reverseT_ = 0;
		frameCount_ = 0;
	}
	else if (state_ == REVERSE) {
		state_ = NONE;
		reverseT_ = 1.0f;
		frameCount_ = kReverseSpeed_;
	}
}

void Screw::OnCollision(ColliderParentObject pairObject, CollisionData collidionData)
{
	if (state_ == FOLLOW || state_ == NONE || state_ == REVERSE) {
		if (std::holds_alternative<Block*>(pairObject)) {
			worldTransform_.transform_.translate.y = std::get<Block*>(pairObject)->GetWorldTransform()->GetWorldPosition().y + std::get<Block*>(pairObject)->GetWorldTransform()->transform_.scale.y + worldTransform_.transform_.scale.y;
			worldTransform_.UpdateMatrix();
		}
	}
}