#include "TitleObj.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/Math/Ease.h"
#include <numbers>

void TitleObj::Initialize(Model* model, const std::string& objName)
{

	model_ = model;

	worldTransform_.Initialize();

	position_[0] = { 0.0f,0.0f,0.0f };
	position_[1] = { 0.0f,0.0f,0.0f };

	moveT_ = 0.0f;

	moveTSpeed_ = 0.0f;

	itIncreaseMoveT_ = true;

	rotate_ = { 0.0f,0.0f,0.0f };

	rotateSpeed_ = { 0.0f,0.0f,0.0f };

	size_ = { 1.0f, 1.0f, 1.0f };

	objName_ = objName;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void TitleObj::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

#endif // _DEBUG

	Fluffy();

	Rotation();

	worldTransform_.transform_.scale = size_;

	worldTransform_.UpdateMatrix();

}

void TitleObj::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera);

}

void TitleObj::DrawOutLine(BaseCamera& camera, OutLineData& outLineData) {
	model_->OutLineDraw(worldTransform_, camera, outLineData);
}

void TitleObj::Fluffy()
{

	if (itIncreaseMoveT_) {
		moveT_ += moveTSpeed_;
		if (moveT_ >= 1.0f) {
			moveT_ = 1.0f;
			itIncreaseMoveT_ = false;
		}
	}
	else {
		moveT_ -= moveTSpeed_;
		if (moveT_ <= 0.0f) {
			moveT_ = 0.0f;
			itIncreaseMoveT_ = true;
		}
	}

	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseInOutQuad, position_[0], position_[1], moveT_);

}

void TitleObj::Rotation()
{

	if (rotateSpeed_.x == 0.0f) {
		worldTransform_.transform_.rotate.x = rotate_.x;
	}
	else {
		worldTransform_.transform_.rotate.x = std::fmodf(worldTransform_.transform_.rotate.x + rotateSpeed_.x, static_cast<float>(std::numbers::pi) * 2.0f);
	}
	if (rotateSpeed_.y == 0.0f) {
		worldTransform_.transform_.rotate.y = rotate_.y;
	}
	else {
		worldTransform_.transform_.rotate.y = std::fmodf(worldTransform_.transform_.rotate.y + rotateSpeed_.y, static_cast<float>(std::numbers::pi) * 2.0f);
	}
	if (rotateSpeed_.z == 0.0f) {
		worldTransform_.transform_.rotate.z = rotate_.z;
	}
	else {
		worldTransform_.transform_.rotate.z = std::fmodf(worldTransform_.transform_.rotate.z + rotateSpeed_.z, static_cast<float>(std::numbers::pi) * 2.0f);
	}

}

void TitleObj::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "TilteObj";

	globalVariables->AddItem(groupName, objName_ + "Position0", position_[0]);
	globalVariables->AddItem(groupName, objName_ + "Position1", position_[1]);
	globalVariables->AddItem(groupName, objName_ + "MoveTSpeed", moveTSpeed_);
	globalVariables->AddItem(groupName, objName_ + "Rotate", rotate_);
	globalVariables->AddItem(groupName, objName_ + "RotateSpeed", rotateSpeed_);
	globalVariables->AddItem(groupName, objName_ + "Size", size_);

}

void TitleObj::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "TilteObj";

	position_[0] = globalVariables->GetVector3Value(groupName, objName_ + "Position0");
	position_[1] = globalVariables->GetVector3Value(groupName, objName_ + "Position1");
	moveTSpeed_ = globalVariables->GetFloatValue(groupName, objName_ + "MoveTSpeed");
	rotate_ = globalVariables->GetVector3Value(groupName, objName_ + "Rotate");
	rotateSpeed_ = globalVariables->GetVector3Value(groupName, objName_ + "RotateSpeed");
	size_ = globalVariables->GetVector3Value(groupName, objName_ + "Size");

}
