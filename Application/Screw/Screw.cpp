#include "Screw.h"
#include "../Player/Player.h"
#include "../Block/Block.h"
#include "../../Engine/Math/Ease.h"
#include "../Block/UFO.h"
#include "../Block/UFOAttract.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../Collider/CollisionConfig.h"
#include "../../Engine/Math/RandomEngine.h"
#include <random>

GameAudioManager* Screw::audioManager_ = nullptr;

const float Screw::rotateSpeedMax_ = 0.01f;

const float Screw::rotateSpeedMin_ = -0.01f;

void(Screw::* Screw::stateTable[])() = {
	&Screw::None,
	& Screw::Follow,
	& Screw::Reverse,
	& Screw::CatchOnPlayer,
	& Screw::ToBlock,
	& Screw::Stuck,
	& Screw::ToPlayer
};

void Screw::StaticInitialize(GameAudioManager* audioManager)
{
	
	audioManager_ = audioManager;

}

void Screw::Initialize() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Screw";

	globalVariables->AddItem(groupName, "StuckMax", kStuckMax);
	globalVariables->AddItem(groupName, "StuckTop", stuckTop_);
	globalVariables->AddItem(groupName, "StuckLow", stuckLow_);
	state_ = State::FOLLOW;
	//state_ = State(5);
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = {0.8f,0.8f,0.8f};
	worldTransform_.direction_ = {0,0,1.0f};
	worldTransform_.transform_.translate = { 0.0f, 6.0f, 0.0f};
	mat_.reset(Material::Create());
	TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	mat_->Update(t, { 1.0f,1.0f,1.0f,1.0f }, 0, 200);

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);
	collider_->SetCollisionAttribute(kCollisionAttributeScrew);
	collider_->SetCollisionMask(!(kCollisionAttributeScrew));
	isDead_ = false;
	isAttract_ = false;
	isRideBlock_ = false;

	followPosition_ = {0};
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-24.0f, -1.0f);
	followPosition_.z = distribution(randomEngine);
	std::uniform_real_distribution<float> distribution2(-12.0f, 12.0f);
	followPosition_.x = distribution2(randomEngine);
	std::uniform_real_distribution<float> distribution3(5.0f, 7.0f);
	followPosition_.y = distribution3(randomEngine);
	std::uniform_real_distribution<float> distribution4(0.5f, 1.2f);
	followSpeed_ = distribution4(randomEngine);
	acceleration_ = { 0 };
	velocity_ = {0};

	matSweat_.reset(Material::Create());
	transformSweatUV_.scale = { 0.5f,1.0f,1.0f };
	transformSweatUV_.rotate = {0};
	transformSweatUV_.translate = {0};
	matSweat_->Update(transformSweatUV_, { 1.0f,1.0f,1.0f,1.0f }, 0, 200);
	worldTransformSweat_.Initialize();

	std::uniform_real_distribution<float> distributionRotateSpeed(rotateSpeedMin_, rotateSpeedMax_);
	rotateSpeed_ = { distributionRotateSpeed(randomEngine), distributionRotateSpeed(randomEngine), distributionRotateSpeed(randomEngine) };

}
void Screw::Update() {
	isDrawSweat_ = false;
	if (worldTransform_.transform_.translate.y >= followPosition_.y) {
		acceleration_.y = -0.001f;
	}
	else {
		acceleration_.y = 0.001f;
	}
	velocity_.x += acceleration_.x;
	velocity_.y += acceleration_.y;
	velocity_.z += acceleration_.z;
	if (!(state_ == NONE || state_ == FOLLOW)) {
		velocity_ = {0};
	}

	worldTransform_.usedDirection_ = false;
	//if (state_ == FOLLOW) {
	//	worldTransform_.usedDirection_ = true;
	//}
	if (1|| state_ != STUCK) {
		GlobalVariables* globalVariables = GlobalVariables::GetInstance();
		const std::string groupName = "Screw";
		kStuckMax = globalVariables->GetIntValue(groupName, "StuckMax");
		stuckTop_ = globalVariables->GetFloatValue(groupName, "StuckTop");
		stuckLow_ = globalVariables->GetFloatValue(groupName, "StuckLow");
	}
	(this->*stateTable[static_cast<size_t>(state_)])();
	worldTransform_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();
	isAttract_ = false;
	isRideBlock_ = false;
}
void Screw::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_, camera,mat_.get());
}

void Screw::DrawOutLine(Model* model, BaseCamera& camera, OutLineData& outLineData) {
	model->OutLineDraw(worldTransform_, camera, outLineData);
}

void Screw::DrawSweat(Model* model, BaseCamera& camera, Matrix4x4& billboard) {
	Matrix4x4 rotateMatrix = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeRotateZMatrix(-(3.141592f/2.0f)),billboard);
	worldTransformSweat_.worldMatrix_ = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeScaleMatrix(worldTransformSweat_.transform_.scale), Matrix4x4Calc::Multiply(rotateMatrix,Matrix4x4Calc::MakeTranslateMatrix(worldTransformSweat_.transform_.translate)));
	//worldTransformSweat_.Map();
	model->Draw(worldTransformSweat_, camera, matSweat_.get(),sweatTextureHandle_);
}

void Screw::Throw(const Vector3 position, void* block, size_t num) {
	state_ = TOBLOCK;
	startPosition_ = position;
	target_ = block;
	worldTransform_.transform_.rotate.z = 0;
	static_cast<Block*>(target_)->SetAnchorPointScrew(num,this);
	worldTransform_.parent_ = nullptr;
	targetNum_ = num;
	frameCount_ = 00;
}

void Screw::Catch(WorldTransform* magnetWorldPosition) {
	state_ = CATCHONPLAYER;
	worldTransform_.transform_.translate = Matrix4x4Calc::Transform(worldTransform_.GetWorldPosition(), Matrix4x4Calc::Inverse(magnetWorldPosition->parentMatrix_));
	worldTransform_.parent_ = magnetWorldPosition;
	startPosition_ = worldTransform_.transform_.translate;
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

	/*if (!isAttract_) {
		worldTransform_.transform_.translate.y -= 0.3f;
		if (worldTransform_.transform_.translate.y <= -20.0f) {
			worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
			worldTransform_.transform_.translate.y = 4.0f;
		}
	}*/
	worldTransform_.transform_.translate.y += velocity_.y;
	if (!isRideBlock_ && worldTransform_.parent_) {
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.parent_ = nullptr;
	}
}

void Screw::Follow() {
	//worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
	if (!isAttract_) {
		Vector3 tPos = Matrix4x4Calc::Transform(followPosition_, player_->GetWorldTransform()->worldMatrix_);
		float distance = Vector3Calc::Length(Vector3Calc::Subtract(worldTransform_.GetWorldPosition(),tPos));
		if (distance >= 0.5f) {
			Vector3 velocity = Vector3Calc::Multiply(kFollowSpeed*followSpeed_, Vector3Calc::Normalize(Vector3Calc::Subtract(Matrix4x4Calc::Transform(followPosition_,player_->GetWorldTransform()->worldMatrix_), worldTransform_.GetWorldPosition())));
			velocity.y = 0;
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity);
		}
		worldTransform_.transform_.translate.y += velocity_.y;
	}
	if (!isRideBlock_ && worldTransform_.parent_) {
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.parent_ = nullptr;
	}

	// 回転
	worldTransform_.transform_.rotate.x = std::fmodf(worldTransform_.transform_.rotate.x + rotateSpeed_.x, 6.28f);
	worldTransform_.transform_.rotate.y = std::fmodf(worldTransform_.transform_.rotate.y + rotateSpeed_.y, 6.28f);
	worldTransform_.transform_.rotate.z = std::fmodf(worldTransform_.transform_.rotate.z + rotateSpeed_.z, 6.28f);

}

void Screw::Reverse() {
	worldTransform_.transform_.rotate.z = reverseT_ * 3.14f;
	reverseT_ = float(frameCount_) / float(kReverseSpeed_);
	reverseT_ = std::clamp(reverseT_, 0.0f, 1.0f);
	
	if (frameCount_ >= kReverseTime && !isAttract_) {
		state_ = NONE;
		reverseT_ = 1.0f;
		frameCount_ = kReverseSpeed_;
	}
	
	frameCount_++;
	if (!isAttract_) {
		worldTransform_.transform_.translate.y -= 0.3f;
		if (worldTransform_.transform_.translate.y <= -20.0f) {
			worldTransform_.transform_.translate = player_->GetWorldTransform()->GetWorldPosition();
			worldTransform_.transform_.translate.y = 4.0f;
		}
	}
	if (!isRideBlock_ && worldTransform_.parent_) {
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.parent_ = nullptr;
	}
}

void Screw::CatchOnPlayer() {
	float t = float(frameCount_) / 15.0f;
	worldTransform_.transform_.translate = Vector3Calc::Add(Vector3Calc::Multiply((1.0f - t), startPosition_), {0,worldTransform_.transform_.scale.y / 2.0f + 2.0f,0});
	//worldTransform_.transform_.translate.y = worldTransform_.transform_.scale.y / 2.0f;
	worldTransform_.transform_.rotate.x = 0;
	worldTransform_.transform_.rotate.z = 3.141592f;
	frameCount_++;
	if (frameCount_ > 15) {
		frameCount_ = 15;
	}
}

void Screw::ToBlock() {
	float t = float(frameCount_) / 30.0f;
	Vector3 endPoint = static_cast<Block*>(target_)->GetAnchorPointWorldPosition(targetNum_);
	//worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseInSine,startPosition_,endPoint,t);
	worldTransform_.transform_.translate = endPoint;
	worldTransform_.transform_.rotate.y = t * 3.14f * 4.0f;
	worldTransform_.usedDirection_ = false;
	//float t = float(kStuckMax - stuckTime_) / float(kStuckMax);
	worldTransform_.transform_.translate.y += (1.0f - t) * -(static_cast<Block*>(target_)->GetWorldTransform()->transform_.scale.y / 2.0f + worldTransform_.transform_.scale.y / 2.0f) + t * (static_cast<Block*>(target_)->GetWorldTransform()->transform_.scale.y / 2.0f + worldTransform_.transform_.scale.y / 2.0f);
	frameCount_--;
	if (frameCount_<0) {
		state_ = STUCK;
		stuckTime_ = kStuckMax;
	}
}

void Screw::Stuck(){
	Vector3 endPoint = static_cast<Block*>(target_)->GetAnchorPointWorldPosition(targetNum_);
	worldTransform_.transform_.translate = endPoint;
	float t = float(kStuckMax - stuckTime_) / float(kStuckMax);
	worldTransform_.transform_.translate.y += (1.0f - t) * (stuckLow_)+t * (stuckTop_);
	worldTransform_.transform_.rotate.y = t * 3.14f * 64.0f;
	isPinch_ = false;
	if (stuckTime_ < float(kStuckMax/3)) {
		isPinch_ = true;
		worldTransform_.transform_.translate.x +=0.2f* RandomEngine::GetRandom(-1.0f,1.0f);
		worldTransform_.transform_.translate.z += 0.2f * RandomEngine::GetRandom(-1.0f, 1.0f);
		worldTransformSweat_.transform_.translate = worldTransform_.transform_.translate;
		worldTransformSweat_.transform_.translate.y += 3.0f;
		sweatAnimationframe_++;
		if (sweatAnimationframe_ > sweatAnimationChange_) {
			sweatAnimationframe_ = 0;
			sweatAnimationNum_++;
		}
		transformSweatUV_.translate.x = (1.0f / float(sweatAnimationMax_)) * sweatAnimationNum_;
		transformSweatUV_.scale.x = 1.0f / float(sweatAnimationMax_);
		matSweat_->SetUvTransform(transformSweatUV_);

		isDrawSweat_ = true;
	}
	else {
		sweatAnimationframe_ = 0;
		sweatAnimationNum_ = 0;
	}
	if (stuckTime_<=0) {
		static_cast<Block*>(target_)->SetAnchorPointScrew(targetNum_, nullptr);
		frameCount_ = 0;
		state_ = NONE;
		worldTransform_.transform_.translate.y += 5.0f;
		audioManager_->PlayWave(kGameAudioNameIndexScrewRemove);
	}
	stuckTime_--;
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
		if (isRideBlock_ == true) {
			state_ = REVERSE;
			reverseT_ = 0;
			frameCount_ = 0;
		}
	}
	else if (state_ == REVERSE) {
		if (isRideBlock_ == true) {
			state_ = NONE;
			reverseT_ = 1.0f;
			frameCount_ = kReverseSpeed_;
		}
	}
	else if (state_ == STUCK) {
		stuckTime_ = kStuckMax;
	}
}

void Screw::OnCollision(ColliderParentObject pairObject, CollisionData collidionData)
{
	//吸われる
	if (state_ == REVERSE || state_ == NONE || state_ == FOLLOW) {
		if (std::holds_alternative<UFOAttract*>(pairObject) && std::get<UFOAttract*>(pairObject)->GetIsAttract()) {
			UFO* ufo = std::get<UFOAttract*>(pairObject)->GetParent();
			Vector3 velocity = Vector3Calc::Multiply(0.1f, Vector3Calc::Normalize(Vector3Calc::Subtract(ufo->GetWorldTransform()->GetWorldPosition(), worldTransform_.transform_.translate)));
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity);
			isAttract_ = true;
		}
	}
	//UFOにぶつかったとき
	if (state_ != REVERSE) {
		if (std::holds_alternative<UFO*>(pairObject)) {
			isDead_ = true;
		}
	}

	//ブロックの上に押し出す
	if (state_ == FOLLOW || state_ == NONE || state_ == REVERSE) {
		if (!std::holds_alternative<UFO*>(pairObject) && std::holds_alternative<Block*>(pairObject) && !isAttract_) {
			worldTransform_.transform_.translate.y = std::get<Block*>(pairObject)->GetWorldTransform()->GetWorldPosition().y + std::get<Block*>(pairObject)->GetWorldTransform()->transform_.scale.y + worldTransform_.transform_.scale.y;
			worldTransform_.UpdateMatrix();
			if (worldTransform_.parent_ != std::get<Block*>(pairObject)->GetWorldTransform()) {
				worldTransform_.transform_.translate = Matrix4x4Calc::Transform(worldTransform_.GetWorldPosition(), Matrix4x4Calc::Inverse(std::get<Block*>(pairObject)->GetWorldTransform()->parentMatrix_));
				worldTransform_.parent_ = std::get<Block*>(pairObject)->GetWorldTransform();
			}
			worldTransform_.UpdateMatrix();
			isRideBlock_=true;
		}
	}
}