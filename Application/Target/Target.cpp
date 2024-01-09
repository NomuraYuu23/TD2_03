#include "Target.h"
#include "../../Engine/base/TextureManager.h"
void Target::Initialize(uint32_t textureHandle, uint32_t textureHandle2[2]) {

	anchor_.reset(Sprite::Create(textureHandle, { 0,0 }, {1.0f,1.0f,1.0f,1.0f}));
	ui_.reset(Sprite::Create(textureHandle2[0], {0,0}, {1.0f,1.0f,1.0f,1.0f}));
	textureHandle_[0] = textureHandle2[0];
	textureHandle_[1] = textureHandle2[1];

}
void Target::Update(std::vector<Block*>* blockList, BaseCamera& camera, Player* player) {
	ForchNearAnchor(blockList,camera,player);
	if (!isTarget_) {
		targetBlock_ = nullptr;
	}
}

bool IsInnerCamera(const Vector3& vector) {
	if (std::abs(vector.x) <= 1.0f && std::abs(vector.y) <= 1.0f && vector.z > 0.0f) {
		return true;
	}
	return false;
}

void Target::ForchNearAnchor(std::vector<Block*>* blockList, BaseCamera& camera, Player* player) {
	isTarget_ = false;
	if (blockList->empty()) {
		isTarget_ = false;
		return;
	}
	std::vector<Block*>::iterator ite = blockList->begin();
	Block* block = *ite;
	size_t num = 0;
	//対象ブロックが一つだったとき
	/*if (blockList->size() == 1) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size();index++) {
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[index].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			bool lengthCheck = Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[num].position, camera.GetTransform()))
			>= Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[index].position,camera.GetTransform()));
			if ((!IsInnerCamera(oldvp) || lengthCheck) && (IsInnerCamera(newvp))) {
				num = index;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1));
				anchor_->SetPosition({ pos.x,pos.y });
				anchor_->SetSize({0.5f,0.5f});
				ui_->SetPosition({ pos.x,pos.y-32.0f });
				ui_->SetSize({ 0.5f,0.5f });
				isTarget_ = true;
			}
		}
		targetBlock_ = block;
		numTargetAnchor_ = num;
		return;
		//現在の情報をいれてリターン
	}*/
	float oldDot=0;
	for (ite; ite != blockList->end();ite++) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size(); index++) {
			if ((*ite)->GetIsCenter()) {
				continue;
			}
			Matrix4x4 view = Matrix4x4Calc::Inverse( player->GetWorldTransform()->worldMatrix_);
			float fovY_ = 0.5f;
			float aspectRatio_ = 1.0f;
			float nearClip_ = 0.01f;
			float farClip_ = 100.0f;
			Matrix4x4 proj = Matrix4x4Calc::MakePerspectiveFovMatrix(fovY_,aspectRatio_,nearClip_,farClip_);
			Matrix4x4 vp = Matrix4x4Calc::Multiply(view,proj);
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 oldpl = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), vp);
			Vector3 newpl = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), vp);
			bool lengthCheck = std::abs(newpl.x) <= std::abs(oldpl.x) && newpl.z > 0.0f;
			lengthCheck = true;
			//>= Vector3Calc::Length(Vector3Calc::Subtract((*ite)->GetAnchorPointArray()[index].position, camera.GetTransform()));
			float dot = Vector3Calc::Dot(Vector3Calc::Normalize(Vector3Calc::Subtract(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_),player->GetWorldTransform()->GetWorldPosition())),Vector3Calc::Normalize(player->GetDirection()));
			if ((!IsInnerCamera(oldpl) || lengthCheck) && dot>0.5f && IsInnerCamera(newvp) && (!isTarget_ || std::cos(dot) < std::cos(oldDot))) {
				num = index;
				block = *ite;
				oldDot = dot;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)); 
				uiNum_ = 0;
				if (block->GetAnchorPointScrew(num)) {
					uiNum_ = 1;
				}
				ui_->SetTextureHandle(textureHandle_[uiNum_]);
				anchor_->SetPosition({ pos.x,pos.y });
				anchor_->SetSize({ 256.0f/4.0f,256.0f/4.0f });
				ui_->SetPosition({ pos.x,pos.y - 64.0f });
				ui_->SetSize({ 135.0f,48.0f });
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
		ui_->Draw();
	}
}