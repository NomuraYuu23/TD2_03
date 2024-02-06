#include "ClearPlayer.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include <numbers>

void ClearPlayer::Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, const std::string& objName)
{

	models_ = models;

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];

	worldTransforms_[kPlayerPartIndexBody].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexBody].transform_.rotate = { 0.0f, 3.14f, 0.0f };
	worldTransforms_[kPlayerPartIndexBody].transform_.translate = { 0.0f, 0.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.translate = { -0.7f, 0.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexRightLeg].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.translate = { 0.7f, 0.0f, 0.0f };

	worldTransforms_[kPlayerPartIndexMagnet].transform_.scale = { 1.0f, 1.0f, 1.0f };
	worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate = { 0.0f, 0.0f, 0.0f };
	worldTransforms_[kPlayerPartIndexMagnet].transform_.translate = { 0.0f, 1.2f, 0.0f };

	size_ = { 1.0f, 1.0f, 1.0f };

	position_ = { 0.0f,0.0f,0.0f };

	objName_ = objName;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void ClearPlayer::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

#endif // _DEBUG

	rotateParameter_ = std::fmodf(rotateParameter_ + rotateParameterSpeed_, 6.28f);

	rotate_.x = std::sinf(rotateParameter_) * rotateMax_;
	worldTransforms_[kPlayerPartIndexBody].transform_.rotate.x = rotate_.x;
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate.x = -rotate_.x;
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate.x = -rotate_.x;

	worldTransforms_[kPlayerPartIndexBody].transform_.translate = position_;

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].transform_.scale = size_;
		worldTransforms_[i].UpdateMatrix();
	}

}

void ClearPlayer::Draw(BaseCamera& camera)
{

	for (uint32_t i = 0; i < models_.size(); i++) {
		models_[i]->Draw(worldTransforms_[i], camera);
	}

}

void ClearPlayer::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "ClearObj";

	globalVariables->AddItem(groupName, objName_ + "RotateParameterSpeed", rotateParameterSpeed_);
	globalVariables->AddItem(groupName, objName_ + "RotateMax", rotateMax_);
	globalVariables->AddItem(groupName, objName_ + "Size", size_);
	globalVariables->AddItem(groupName, objName_ + "Positon", position_);

}

void ClearPlayer::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "ClearObj";

	rotateParameterSpeed_ = globalVariables->GetFloatValue(groupName, objName_ + "RotateParameterSpeed");
	rotateMax_ = globalVariables->GetFloatValue(groupName, objName_ + "RotateMax");
	size_ = globalVariables->GetVector3Value(groupName, objName_ + "Size");
	position_ = globalVariables->GetVector3Value(groupName, objName_ + "Positon");

}
