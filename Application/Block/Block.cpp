#include "Block.h"
#include "UFO.h"
#include "../Player/player.h"
#include "../Collider/CollisionConfig.h"
void Block::Initialize() {
	anchorPoints_[0].position = {  0.0f, 1.0f, 0.0f };
	//anchorPoints_[1].position = { -0.8f, 1.0f, 0.8f };
	//anchorPoints_[2].position = {  0.8f, 1.0f,-0.8f };
	//anchorPoints_[3].position = { -0.8f, 1.0f,-0.8f };
	for (int index = 0; index < anchorNum; index++) {
		//anchorPoints_[index].isSting = false;
		anchorPoints_[index].screw = nullptr;
	}
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = {2.0f,1.0f,2.0f};

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate,worldTransform_.rotateMatrix_,worldTransform_.transform_.scale,this);
	collider_->SetCollisionAttribute(kCollisionAttributeBlock);
	isConnect_ = false;
	isCenter_ = false;
	velocity_ = { 0,0,0};
	isRelese_ = false;
	isRidePlayer_ = false;
}
void Block::Update() {
	isRelese_ = true;
	if (!isConnect_) {
		worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,velocity_);
	}
	if (isConnect_) {
		bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr) {
				isStack = true;
				break;
			}
		}

		if (!isStack && !isCenter_) {
			isConnect_ = false;
			isRelese_ = true;
		}
	}
	worldTransform_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->size_ = { worldTransform_.transform_.scale.x * 3.0f, worldTransform_.transform_.scale.y / 2.0f, worldTransform_.transform_.scale.z * 3.0f };
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();

	//isCenter_ = false;
	isRidePlayer_ = false;
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera);
}

Vector3 Block::GetAnchorPointWorldPosition(size_t num) {
	return Matrix4x4Calc::Transform(anchorPoints_[num].position,worldTransform_.worldMatrix_);
}

void Block::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (!isConnect_ && std::holds_alternative<Magnet*>(pairObject)) {
		bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr && anchorPoints_[index].screw->GetState() == Screw::STUCK) {
				isStack = true;
				break;
			}
		}
		if (isStack && !isRidePlayer_) {
			Vector3 toPlayer = Vector3Calc::Subtract(std::get<Magnet*>(pairObject)->GetCenter(), worldTransform_.GetWorldPosition());
			toPlayer.y = 0;
			toPlayer = Vector3Calc::Normalize(toPlayer);
			toPlayer = Vector3Calc::Multiply(0.5f, toPlayer);
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, toPlayer);
		}
	}

	if (!isCenter_ && !isConnect_ && (std::holds_alternative<Block*>(pairObject) || (std::holds_alternative<UFO*>(pairObject)))) {
		bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr && anchorPoints_[index].screw->GetState() == Screw::STUCK) {
				isStack = true;
				break;
			}
		}
		if (isStack && std::get<Block*>(pairObject)->GetIsConnect()) {
			/*Matrix4x4 rocal = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_, (Matrix4x4Calc::Inverse(std::get<Block*>(pairObject)->GetWorldTransform()->worldMatrix_)));
			worldTransform_.transform_.translate.x = rocal.m[3][0];
			worldTransform_.transform_.translate.y = rocal.m[3][1];
			worldTransform_.transform_.translate.z = rocal.m[3][2];
			worldTransform_.p*/
			isConnect_ = true;
		}
	}
	if (std::holds_alternative<Player*>(pairObject)) {
		isRidePlayer_ = true;
	}
}
