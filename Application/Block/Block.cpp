#include "Block.h"

void Block::Initialize() {
	anchorPoints_[0].position = {  0.8f, 1.0f, 0.8f };
	anchorPoints_[1].position = { -0.8f, 1.0f, 0.8f };
	anchorPoints_[2].position = {  0.8f, 1.0f,-0.8f };
	anchorPoints_[3].position = { -0.8f, 1.0f,-0.8f };
	for (int index = 0; index < 4; index++) {
		anchorPoints_[index].isStings = false;
		anchorPoints_[index].screw = nullptr;
	}
	worldTransform_.Initialize();
}
void Block::Update() {

	worldTransform_.UpdateMatrix();
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera);
}
