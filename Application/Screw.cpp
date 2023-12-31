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
}
void Screw::Update() {
	(this->*stateTable[static_cast<size_t>(state_)])();
	worldTransform_.UpdateMatrix();
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

void Screw::Follow() {
	worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
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

