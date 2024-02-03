#include "PlayerAnimation.h"
#include <numbers>
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../player.h"
#include "../../Block/Block.h"

std::array<PlayerAnimation::AnimationData, PlayerAnimation::GravityPhaseIndex::kGravityPhaseIndexOfCount> PlayerAnimation::gravityAnimationData_;

std::array<PlayerAnimation::AnimationData, PlayerAnimation::ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount> PlayerAnimation::screwThrowingAnimationData_;

void PlayerAnimation::Initialize(Player* player)
{

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	assert(player);
	player_ = player;

	worldTransforms_[kPlayerPartIndexBody].parent_ = player_->GetWorldTransform();
	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];

	TransformInitialize();

	prevPlayerAnimationNo_ = kPlayerAnimationIndexStand;

	AnimationInitialize();

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
		case kPlayerAnimationIndexGravity:
			GravityInitialize();
			break;
		case kPlayerAnimationIndexScrewThrowing:
			ScrewThrowingInitialize();
			break;
		case kPlayerAnimationIndexFalling:
			FallingInitialize();
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
	case kPlayerAnimationIndexGravity:
		GravityUpdate();
		break;
	case kPlayerAnimationIndexScrewThrowing:
		ScrewThrowingUpdate();
		break;
	case kPlayerAnimationIndexFalling:
		FallingUpdate();
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

void PlayerAnimation::AnimationInitialize()
{
	
	for (uint32_t i = 0; i < GravityPhaseIndex::kGravityPhaseIndexOfCount; ++i) {
		for (uint32_t j = 0; j < PlayerPartIndex::kPlayerPartIndexOfCount; ++j) {
			gravityAnimationData_[i].transforms_[j].scale = {1.0f,1.0f,1.0f};
			gravityAnimationData_[i].transforms_[j].rotate = { 0.0f,0.0f,0.0f };
			gravityAnimationData_[i].transforms_[j].translate = { 0.0f,0.0f,0.0f };
		}
		gravityAnimationData_[i].frame_ = 10;
		gravityAnimationData_[i].easeName_ = Ease::EaseName::Lerp;

	}

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

	workWalk_.rotate_ = std::fmodf(workWalk_.rotate_ + workWalk_.speed_, 6.28f);
	
	float radian = static_cast<float>(workWalk_.angle_) * static_cast<float>(std::numbers::pi) / 180.0f;

	float leftRotate = -std::sinf(workWalk_.rotate_) * radian;
	float righttRotate = std::sinf(workWalk_.rotate_) * radian;

	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.rotate.x = leftRotate;
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.rotate.x = righttRotate;

	float height = -0.5f;

	worldTransforms_[kPlayerPartIndexBody].transform_.translate.y = std::fabsf(std::cosf(workWalk_.rotate_)) * height + 2.0f;

	worldTransforms_[kPlayerPartIndexMagnet].transform_.rotate.y = workWalk_.rotate_;

}

void PlayerAnimation::GravityInitialize()
{

	TransformInitialize();

	workGravity_.frameCount_ = 0;
	workGravity_.phaseNum_ = 0;

	workGravity_.frame_ = gravityAnimationData_[workGravity_.phaseNum_].frame_;
	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		workGravity_.currentTransforms_[i] = worldTransforms_[i].transform_;
		workGravity_.nextTransforms_[i] = gravityAnimationData_[workGravity_.phaseNum_].transforms_[i];
	}
	workGravity_.easeName_ = gravityAnimationData_[workGravity_.phaseNum_].easeName_;

}

void PlayerAnimation::GravityUpdate()
{

	if (workGravity_.phaseNum_ == GravityPhaseIndex::kGravityPhaseIndexOfCount || workGravity_.frame_ == 0) {
		return;
	}

	float t = static_cast<float>(workGravity_.frameCount_) / static_cast<float>(workGravity_.frame_);

	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		worldTransforms_[i].transform_.scale = 
			Ease::Easing(workGravity_.easeName_, workGravity_.currentTransforms_[i].scale, workGravity_.nextTransforms_[i].scale, t);
		worldTransforms_[i].transform_.rotate =
			Ease::Easing(workGravity_.easeName_, workGravity_.currentTransforms_[i].rotate, workGravity_.nextTransforms_[i].rotate, t);
		worldTransforms_[i].transform_.translate =
			Ease::Easing(workGravity_.easeName_, workGravity_.currentTransforms_[i].translate, workGravity_.nextTransforms_[i].translate, t);
	}

	if (workGravity_.frameCount_++ == workGravity_.frame_ && workGravity_.phaseNum_ < GravityPhaseIndex::kGravityPhaseIndexOfCount) {
		workGravity_.phaseNum_++;
		workGravity_.frameCount_ = 0;
		if (workGravity_.phaseNum_ == GravityPhaseIndex::kGravityPhaseIndexOfCount) {
			return;
		}
		workGravity_.frame_ = gravityAnimationData_[workGravity_.phaseNum_].frame_;

		for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
			workGravity_.currentTransforms_[i] = worldTransforms_[i].transform_;
			workGravity_.nextTransforms_[i] = gravityAnimationData_[workGravity_.phaseNum_].transforms_[i];
		}
		workGravity_.easeName_ = gravityAnimationData_[workGravity_.phaseNum_].easeName_;
	}

}

void PlayerAnimation::ScrewThrowingInitialize()
{

	TransformInitialize();

	workScrewThrowing_.frameCount_ = 0;
	workScrewThrowing_.phaseNum_ = 0;

	workScrewThrowing_.frame_ = screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].frame_;
	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		workScrewThrowing_.currentTransforms_[i] = worldTransforms_[i].transform_;
		workScrewThrowing_.nextTransforms_[i] = screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].transforms_[i];
	}
	workScrewThrowing_.easeName_ = screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].easeName_;

}

void PlayerAnimation::ScrewThrowingUpdate()
{

	if (workScrewThrowing_.phaseNum_ == ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount || workScrewThrowing_.frame_ == 0) {
		return;
	}

	// ブロックの位置をとる
	if (workScrewThrowing_.phaseNum_ == ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexThrowing ||
		workScrewThrowing_.phaseNum_ == ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexTurn) {
		ScrewThrowingMagunetException();
	}

	float t = static_cast<float>(workScrewThrowing_.frameCount_) / static_cast<float>(workScrewThrowing_.frame_);

	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		worldTransforms_[i].transform_.scale =
			Ease::Easing(workScrewThrowing_.easeName_, workScrewThrowing_.currentTransforms_[i].scale, workScrewThrowing_.nextTransforms_[i].scale, t);
		worldTransforms_[i].transform_.rotate =
			Ease::Easing(workScrewThrowing_.easeName_, workScrewThrowing_.currentTransforms_[i].rotate, workScrewThrowing_.nextTransforms_[i].rotate, t);
		worldTransforms_[i].transform_.translate =
			Ease::Easing(workScrewThrowing_.easeName_, workScrewThrowing_.currentTransforms_[i].translate, workScrewThrowing_.nextTransforms_[i].translate, t);
	}

	if (workScrewThrowing_.frameCount_++ == workScrewThrowing_.frame_ && workScrewThrowing_.phaseNum_ < ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount) {
		workScrewThrowing_.phaseNum_++;
		workScrewThrowing_.frameCount_ = 0;
		if (workScrewThrowing_.phaseNum_ == ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount) {
			//デバッグ
			ScrewThrowingInitialize();
			return;
		}
		workScrewThrowing_.frame_ = screwThrowingAnimationData_[workGravity_.phaseNum_].frame_;

		for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
			workScrewThrowing_.currentTransforms_[i] = worldTransforms_[i].transform_;
			workScrewThrowing_.nextTransforms_[i] = screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].transforms_[i];
		}
		workScrewThrowing_.easeName_ = screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].easeName_;
	}

}

void PlayerAnimation::ScrewThrowingMagunetException()
{

	Matrix4x4 worldMatrix = Matrix4x4Calc::MakeAffineMatrix(
		screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].transforms_[PlayerPartIndex::kPlayerPartIndexBody].scale,
		screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].transforms_[PlayerPartIndex::kPlayerPartIndexBody].rotate,
		screwThrowingAnimationData_[workScrewThrowing_.phaseNum_].transforms_[PlayerPartIndex::kPlayerPartIndexBody].translate);
	
	worldMatrix = Matrix4x4Calc::Multiply(worldMatrix, player_->GetWorldTransform()->parentMatrix_);

	Matrix4x4 worldMatrixInverse = Matrix4x4Calc::Inverse(worldMatrix);

	Vector3 blockPosition = player_->GetBlock()->GetAnchorPointWorldPosition(0);
	blockPosition.y += workScrewThrowing_.blockAddPositionY_;

	Vector3 translate = Matrix4x4Calc::Transform(blockPosition, worldMatrixInverse);

	workScrewThrowing_.nextTransforms_[PlayerPartIndex::kPlayerPartIndexMagnet].translate = translate;
}

void PlayerAnimation::FallingInitialize()
{

	TransformInitialize();

	workFalling_.parameter_ = 0.0f;

}

void PlayerAnimation::FallingUpdate()
{

	workFalling_.parameter_ =  std::clamp(workFalling_.parameter_ + workFalling_.speed_, 0.0f, 1.0f);

	worldTransforms_[kPlayerPartIndexBody].transform_.rotate = 
		Ease::Easing(Ease::EaseName::Lerp, workFalling_.startRotate_, workFalling_.endRotate_, workFalling_.parameter_);

}

void PlayerAnimation::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// 歩き状態
	std::string groupName = "WalkPlayerAnimation";
	globalVariables->AddItem(groupName, "WalkSpeed", workWalk_.speed_);
	globalVariables->AddItem(groupName, "WalkAngle", workWalk_.angle_);

	// 重力状態
	groupName = "GravityPlayerAnimation";
	for (uint32_t i = 0; i < GravityPhaseIndex::kGravityPhaseIndexOfCount; ++i) {

		for (uint32_t j = 0; j < PlayerPartIndex::kPlayerPartIndexOfCount; ++j) {
			globalVariables->AddItem(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Scale", gravityAnimationData_[i].transforms_[j].scale);
			globalVariables->AddItem(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Rotate", gravityAnimationData_[i].transforms_[j].rotate);
			globalVariables->AddItem(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Translate", gravityAnimationData_[i].transforms_[j].translate);
		}
		globalVariables->AddItem(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + "Frame", gravityAnimationData_[i].frame_);
		globalVariables->AddItem(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + "EaseName", static_cast<uint32_t>(gravityAnimationData_[i].easeName_));
	}

	// ねじ投擲状態
	groupName = "ScrewThrowingPlayerAnimation";
	for (uint32_t i = 0; i < ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount; ++i) {

		for (uint32_t j = 0; j < PlayerPartIndex::kPlayerPartIndexOfCount; ++j) {
			globalVariables->AddItem(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Scale", screwThrowingAnimationData_[i].transforms_[j].scale);
			globalVariables->AddItem(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Rotate", screwThrowingAnimationData_[i].transforms_[j].rotate);
			globalVariables->AddItem(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Translate", screwThrowingAnimationData_[i].transforms_[j].translate);
		}
		globalVariables->AddItem(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + "Frame", screwThrowingAnimationData_[i].frame_);
		globalVariables->AddItem(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + "EaseName", static_cast<uint32_t>(screwThrowingAnimationData_[i].easeName_));
	}
	globalVariables->AddItem(groupName, "BlockAddPositionY", workScrewThrowing_.blockAddPositionY_);

}

void PlayerAnimation::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	
	// 歩き状態
	std::string groupName = "WalkPlayerAnimation";
	workWalk_.speed_ = globalVariables->GetFloatValue(groupName, "WalkSpeed");
	workWalk_.angle_ = globalVariables->GetIntValue(groupName, "WalkAngle");

	// 重力状態
	groupName = "GravityPlayerAnimation";
	for (uint32_t i = 0; i < GravityPhaseIndex::kGravityPhaseIndexOfCount; ++i) {

		for (uint32_t j = 0; j < PlayerPartIndex::kPlayerPartIndexOfCount; ++j) {
			gravityAnimationData_[i].transforms_[j].scale = globalVariables->GetVector3Value(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Scale");
			gravityAnimationData_[i].transforms_[j].rotate = globalVariables->GetVector3Value(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Rotate");
			gravityAnimationData_[i].transforms_[j].translate = globalVariables->GetVector3Value(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Translate");
		}
		gravityAnimationData_[i].frame_ = globalVariables->GetUIntValue(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + "Frame");
		gravityAnimationData_[i].easeName_ = static_cast<Ease::EaseName>(globalVariables->GetUIntValue(groupName, "GravityAnimationData" + kGravityPhaseIndexNames_[i] + "EaseName"));
	}

	// ねじ投擲状態
	groupName = "ScrewThrowingPlayerAnimation";
	for (uint32_t i = 0; i < ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount; ++i) {

		for (uint32_t j = 0; j < PlayerPartIndex::kPlayerPartIndexOfCount; ++j) {
			screwThrowingAnimationData_[i].transforms_[j].scale = globalVariables->GetVector3Value(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Scale");
			screwThrowingAnimationData_[i].transforms_[j].rotate = globalVariables->GetVector3Value(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Rotate");
			screwThrowingAnimationData_[i].transforms_[j].translate = globalVariables->GetVector3Value(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + kPlayerPartIndexNames_[j] + "Translate");
		}
		screwThrowingAnimationData_[i].frame_ = globalVariables->GetUIntValue(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + "Frame");
		screwThrowingAnimationData_[i].easeName_ = static_cast<Ease::EaseName>(globalVariables->GetUIntValue(groupName, "ScrewThrowingAnimationData" + kScrewThrowingPhaseIndexNames_[i] + "EaseName"));
	}
	workScrewThrowing_.blockAddPositionY_ = globalVariables->GetFloatValue(groupName, "BlockAddPositionY");

}
