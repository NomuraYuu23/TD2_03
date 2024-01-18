#include "PlayerAnimation.h"
#include <numbers>
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PlayerAnimation::Initialize(WorldTransform* worldTransform)
{

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	playerWorldTransform_ = worldTransform;
	assert(playerWorldTransform_);

	worldTransforms_[kPlayerPartIndexBody].parent_ = playerWorldTransform_;
	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];

	TransformInitialize();

	prevPlayerAnimationNo_ = kPlayerAnimationIndexStand;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void PlayerAnimation::Update(PlayerAnimationIndex playerAnimationNo)
{

#ifdef _DEBUG

	ApplyGlobalVariables();

#endif // _DEBUG


	if (prevPlayerAnimationNo_ != playerAnimationNo) {
		switch (playerAnimationNo)
		{
		case kPlayerAnimationIndexStand:
			StandInitialize();
			break;
		case kPlayerAnimationIndexWalk:
			WalkInitialize();
			break;
		case kPlayerAnimationIndexOfCount:
			assert(0);
			break;
		default:
			break;
		}
		prevPlayerAnimationNo_ = playerAnimationNo;
	}

	switch (playerAnimationNo)
	{
	case kPlayerAnimationIndexStand:
		StandUpdate();
		break;
	case kPlayerAnimationIndexWalk:
		WalkUpdate();
		break;
	case kPlayerAnimationIndexOfCount:
		assert(0);
		break;
	default:
		break;
	}

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].UpdateMatrix();
	}

}

void PlayerAnimation::Draw(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, BaseCamera& camera, Material* mat)
{

	for (uint32_t i = 0; i < models.size(); i++) {
		models[i]->Draw(worldTransforms_[i], camera, mat);
	}

}

void PlayerAnimation::ImGuiDraw()
{

}

void PlayerAnimation::TransformInitialize()
{

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

}

void PlayerAnimation::StandInitialize()
{

	TransformInitialize();

}

void PlayerAnimation::StandUpdate()
{

}

void PlayerAnimation::WalkInitialize()
{

	TransformInitialize();
	workWalk_.rotate_ = 0.0f;

}

void PlayerAnimation::WalkUpdate()
{

	workWalk_.rotate_ = std::fmodf(workWalk_.rotate_ + workWalk_.speed_, 6.24f);
	
	float radian = static_cast<float>(workWalk_.angle_) * static_cast<float>(std::numbers::pi) / 180.0f;

	float leftRotate = -std::sinf(workWalk_.rotate_) * radian;
	float righttRotate = std::sinf(workWalk_.rotate_) * radian;

	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate.x = leftRotate;
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate.x = righttRotate;

}

void PlayerAnimation::GravityInitialize()
{

	TransformInitialize();

}

void PlayerAnimation::GravityUpdate()
{


}

void PlayerAnimation::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "PlayerAnimation";

	// 歩き状態
	globalVariables->AddItem(groupName, "WalkSpeed", workWalk_.speed_);
	globalVariables->AddItem(groupName, "WalkAngle", workWalk_.angle_);

}

void PlayerAnimation::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "PlayerAnimation";

	// 歩き状態
	workWalk_.speed_ = globalVariables->GetFloatValue(groupName, "WalkSpeed");
	workWalk_.angle_ = globalVariables->GetIntValue(groupName, "WalkAngle");

}
