#include "Block.h"

void Block::Initialize() {
	anchorPoints_[0].position = {  0.8f, 1.0f, 0.8f };
	anchorPoints_[1].position = { -0.8f, 1.0f, 0.8f };
	anchorPoints_[2].position = {  0.8f, 1.0f,-0.8f };
	anchorPoints_[3].position = { -0.8f, 1.0f,-0.8f };
	for (int index = 0; index < 4; index++) {
		//anchorPoints_[index].isSting = false;
		anchorPoints_[index].screw = nullptr;
	}
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = {3.0f,0.5f,3.0f};

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate,worldTransform_.rotateMatrix_,worldTransform_.transform_.scale,this);
	isConnect_ = false;
}
void Block::Update() {

	worldTransform_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera);
}

Vector3 Block::GetAnchorPointWorldPosition(size_t num) {
	return Matrix4x4Calc::Transform(anchorPoints_[num].position,worldTransform_.worldMatrix_);
}

void Block::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (!isConnect_ && std::holds_alternative<Player*>(pairObject)) {
		Vector3 toPlayer = Vector3Calc::Subtract( collidionData.p2 , worldTransform_.GetWorldPosition());
		toPlayer.y = 0;
		toPlayer = Vector3Calc::Normalize(toPlayer);
		toPlayer = Vector3Calc::Multiply(1.0f,toPlayer);
		worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,toPlayer);
	}
}
