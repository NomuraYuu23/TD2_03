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
}
void Block::Update() {

	worldTransform_.UpdateMatrix();
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera);
}

Vector3 Block::GetAnchorPointWorldPosition(size_t num) {
	return Matrix4x4Calc::Transform(anchorPoints_[num].position,worldTransform_.worldMatrix_);
}