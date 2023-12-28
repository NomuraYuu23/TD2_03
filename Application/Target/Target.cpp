#include "Target.h"
#include "../../Engine/base/TextureManager.h"
void Target::Initialize(uint32_t textureHandle) {

	anchor_.reset(Sprite::Create(textureHandle, { 0,0 }, {1.0f,1.0f,1.0f,1.0f}));
}
void Target::Update(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera) {
	ForchNearAnchor(blockList,camera);
}

bool IsInnerCamera(const Vector3& vector) {
	if (std::abs(vector.x) <= 1.0f && std::abs(vector.y) <= 1.0f && vector.z > 0.0f) {
		return true;
	}
	return false;
}

void Target::ForchNearAnchor(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera) {
	isTarget_ = false;
	if (blockList->empty()) {
		isTarget_ = false;
		return;
	}
	std::vector<std::unique_ptr<Block>>::iterator ite = blockList->begin();
	Block* block = ite->get();
	size_t num = 0;
	//対象ブロックが一つだったとき
	if (blockList->size() == 1) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size();index++) {
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[index].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			bool lengthCheck = Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[num].position, camera.GetTransform()))
			>= Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[index].position,camera.GetTransform()));
			if ((!IsInnerCamera(oldvp) || lengthCheck) && (IsInnerCamera(newvp))) {
				num = index;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1));
				anchor_->SetPosition({ pos.x,pos.y });
				isTarget_ = true;
			}
		}
		targetBlock_ = block;
		numTargetAnchor_ = num;
		return;
		//現在の情報をいれてリターン
	}
	for (ite; ite != blockList->end();ite++) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size(); index++) {
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			bool lengthCheck = Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[num].position, camera.GetTransform()))
			>= Vector3Calc::Length(Vector3Calc::Subtract((*ite)->GetAnchorPointArray()[index].position, camera.GetTransform()));
			if ((!IsInnerCamera(oldvp) || lengthCheck) && (IsInnerCamera(newvp))) {
				num = index;
				block = ite->get();
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)); 
				anchor_->SetPosition({ pos.x,pos.y });
				isTarget_ = true;
			}
		}
	}
	targetBlock_ = block;
	numTargetAnchor_ = num;
	return;
}

void Target::SpriteDraw() {
	if (isTarget_) {
		anchor_->Draw();
	}
}