#include "PlayerAnimation.h"
#include <numbers>

void PlayerAnimation::Initialize(WorldTransform* worldTransform)
{

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	playerWorldTransform_ = worldTransform;
	assert(playerWorldTransform_);

	worldTransforms_[kPlayerPartIndexBody].parent_ = playerWorldTransform_;
	worldTransforms_[kPlayerPartIndexBody].transform_.translate.y = 2.0f;
	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.translate.x = -0.7f;
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.translate.x = 0.7f;
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].transform_.translate.y = 1.2f;

	prevPlayerAnimationNo_ = kPlayerAnimationIndexStand;

}

void PlayerAnimation::Update(PlayerAnimationIndex playerAnimationNo)
{

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

void PlayerAnimation::StandInitialize()
{

	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate.x = 0.0f;
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate.x = 0.0f;

}

void PlayerAnimation::StandUpdate()
{

}

void PlayerAnimation::WalkInitialize()
{

	workWalk_.rotate_ = 0.0f;
	workWalk_.speed_ = 0.1f;
	workWalk_.angle_ = 60;

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
