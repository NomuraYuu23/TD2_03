#include "Block.h"

void Block::Initialize() {
	anchorPoints_[0].position = { -1.0f,0.0f,0.0f };
	anchorPoints_[1].position = {  1.0f,0.0f,0.0f };
	anchorPoints_[2].position = { 0.0f,-1.0f,0.0f };
	anchorPoints_[3].position = { 0.0f, 1.0f,0.0f };
	anchorPoints_[4].position = { 0.0f,0.0f,-1.0f };
	anchorPoints_[5].position = { 0.0f,0.0f, 1.0f };
	for (int index = 0; index < 6; index++) {
		anchorPoints_[index].isStings = false;
	}
	worldTransform_.Initialize();
}
void Block::Update() {

	worldTransform_.UpdateMatrix();
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera);
}
