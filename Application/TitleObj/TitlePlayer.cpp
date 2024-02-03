#include "TitlePlayer.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include <numbers>

void TitlePlayer::Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, const std::string& objName)
{

	models_ = models;

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];

	worldTransforms_[kPlayerPartIndexBody].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexBody].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexBody].transform_.translate = { 0.0f, 2.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.translate = { -0.7f, 0.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexRightLeg].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.translate = { 0.7f, 0.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexMagnet].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexMagnet].transform_.translate = { 0.0f, 1.2f, 0.0f };

	position_[0] = { 0.0f,0.0f,0.0f };
	position_[1] = { 0.0f,0.0f,0.0f };

	leftLegRotate_[0] = { 0.0f,0.0f,-0.3925f };
	leftLegRotate_[1] = { 0.0f,0.0f,0.0f };

	rightLegRotate_[0] = { 0.0f,0.0f,0.3925f };
	rightLegRotate_[1] = { 0.0f,0.0f,0.0f };

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

void TitlePlayer::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

#endif // _DEBUG

	Fluffy();

	Rotation();

	worldTransforms_[kPlayerPartIndexBody].transform_.rotate = rotate_;

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].transform_.scale = size_;
		worldTransforms_[i].UpdateMatrix();
	}

}

void TitlePlayer::Draw(BaseCamera& camera)
{

	for (uint32_t i = 0; i < models_.size(); i++) {
		models_[i]->Draw(worldTransforms_[i], camera);
	}

}

void TitlePlayer::Fluffy()
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

	worldTransforms_[kPlayerPartIndexBody].transform_.translate = Ease::Easing(Ease::EaseName::EaseInOutQuad, position_[0], position_[1], moveT_);
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate = Ease::Easing(Ease::EaseName::EaseInOutQuad, leftLegRotate_[0], leftLegRotate_[1], moveT_);
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate = Ease::Easing(Ease::EaseName::EaseInOutQuad, rightLegRotate_[0], rightLegRotate_[1], moveT_);

}

void TitlePlayer::Rotation()
{

	if (rotateSpeed_.x == 0.0f) {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.x = 0.0f;
	}
	else {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.x = std::fmodf(worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.x + rotateSpeed_.x, static_cast<float>(std::numbers::pi) * 2.0f);
	}
	if (rotateSpeed_.y == 0.0f) {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.y = 0.0f;
	}
	else {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.y = std::fmodf(worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.y + rotateSpeed_.y, static_cast<float>(std::numbers::pi) * 2.0f);
	}
	if (rotateSpeed_.z == 0.0f) {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.z = 0.0f;
	}
	else {
		worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.z = std::fmodf(worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.z + rotateSpeed_.z, static_cast<float>(std::numbers::pi) * 2.0f);
	}

}

void TitlePlayer::RegisteringGlobalVariables()
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

void TitlePlayer::ApplyGlobalVariables()
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
