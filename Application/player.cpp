#include "Player.h"
//#include "ImGuiManager.h"

#include <algorithm>
#include <cassert>
#include <numbers>

//#include "GlobalVariables.h"

//#include "CollisionManager.h"
//#include "RandomEngine.h"

static int startFrame = 0;
static int endFrame = 40;
static int rigidityFrame = 30;
static int attackFrame = 15;


void Player::Initialize(const std::vector<HierarchicalAnimation>& models) {
	
	models_ = models;
	worldTransform_.Initialize();
	
	input_ = Input::GetInstance();
	direction_ = { 0,0,1.0f };
	directionMatrix_ = MakeIdentity4x4();

	for (HierarchicalAnimation& model_ : models_) {
		model_.worldTransform_.Initialize();
		model_.worldTransform_.UpdateMatrix();
	}
	std::vector<HierarchicalAnimation>::iterator body = models_.begin();
	body->worldTransform_.parent_ = &worldTransform_;
	for (std::vector<HierarchicalAnimation>::iterator childlen = models_.begin() + 1;
		childlen != models_.end(); childlen++) {
		childlen->worldTransform_.parent_ = &(body->worldTransform_);
	}
	
}


void Player::BehaviorRootInitialize() {
	InitializeFloatingGimmick();
	models_[2].worldTransform_.parent_ = &models_[0].worldTransform_;
	models_[3].worldTransform_.parent_ = &models_[0].worldTransform_;
	models_[2].worldTransform_.transform_.translate.x = -0.5f;
	models_[2].worldTransform_.transform_.translate.y = 1.5f;
	models_[2].worldTransform_.transform_.translate.z = 0.0f;
	models_[3].worldTransform_.transform_.translate.x = 0.5f;
	models_[3].worldTransform_.transform_.translate.y = 1.5f;
	models_[3].worldTransform_.transform_.translate.z = 0.0f;

	models_[2].worldTransform_.transform_.rotate.x = 0.0f;
	models_[2].worldTransform_.transform_.rotate.y = 0.0f;
	models_[3].worldTransform_.transform_.rotate.x = 0.0f;
	models_[3].worldTransform_.transform_.rotate.y = 0.0f;
	comboNum_ = 0;
}



void Player::Update() {
	ApplyGlobalVariables();
	Input::GetInstance()->GetJoystickState(0, joyState_);
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		frameCount_ = 0;
		switch (behavior_) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		
		}
		behaviorRequest_ = std::nullopt;
	}
	//BehaviorRootUpdate();
	//BehaviorAttackUpdate();
	if (worldTransform_.parent_) {
		if (worldTransform_.translation_.y < 0.0f) {
			worldTransform_.translation_.y = 0.0f;
		}

	}


	switch (behavior_) {
	case Player::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}
	/*
	if (worldTransform_.translation_.y < -10.0f) {
		//ReStart();
	}*/

	// 行列更新
	//worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeScaleMatrix(worldTransform_.scale_) * directionMatrix_ * MakeTranslateMatrix(worldTransform_.translation_);
	if (worldTransform_.parent_) {
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}
	// キャラクタの座標を表示する
	float* slider3[3] = {
		&worldTransform_.translation_.x, &worldTransform_.translation_.y,
		&worldTransform_.translation_.z };
	/*
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::SliderFloat3("", *slider3, -100.0f, 100.0f);
	ImGui::SliderInt("Attack", &attackFrame, 1, 240);
	ImGui::SliderInt("Rigd", &rigidityFrame, 1, 240);
	ImGui::SliderInt("End", &endFrame, 1, 240);
	ImGui::End();
#endif // _DEBUG
*/

	for (HierarchicalAnimation& model : models_) {
		model.worldTransform_.UpdateMatrix();
	}

	preJoyState_ = joyState_;
}

void Player::BehaviorRootUpdate()
{
	// ゲームパッドの状態をえる
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		//behavior_ = Behavior::kAttack;
		behaviorRequest_ = Behavior::kAttack;
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X))
	{
		//behavior_ = Behavior::kAttack;
		behaviorRequest_ = Behavior::kDash;
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
		worldTransform_.parent_)
	{
		behaviorRequest_ = Behavior::kJump;
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float kCharacterSpeed = 0.3f;

		Vector3 move = {
			float(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0,
			float(joyState.Gamepad.sThumbLY) / SHRT_MAX };
		//Matrix4x4 newrotation = DirectionToDIrection({0,0.0f,1.0f}, {0, 0.0f, -1.0f});
		move = Normalize(move) * kCharacterSpeed;
		//Vector3 cameraDirectionYcorection = {0.0f, viewProjection_->matView.m[1][0] * viewProjection_->matView.m[1][0]* viewProjection_->matView.m[1][2], 0.0f};
		Matrix4x4 cameraRotateY = Inverse(viewProjection_->matView);
		//cameraRotateY.m[0][0] = 1;
		cameraRotateY.m[0][1] = 0;
		//cameraRotateY.m[0][2] = 0;

		cameraRotateY.m[1][0] = 0;
		cameraRotateY.m[1][1] = 1;
		cameraRotateY.m[1][2] = 0;


		//cameraRotateY.m[2][0] = 0;
		cameraRotateY.m[2][1] = 0;
		//cameraRotateY.m[2][2] = 1;

		cameraRotateY.m[3][0] = 0;
		cameraRotateY.m[3][1] = 0;
		cameraRotateY.m[3][2] = 0;
		//cameraRotateY = Inverse(cameraRotateY);
		move = Transform(move, cameraRotateY);
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0) {
			//worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			Matrix4x4 newDirection = DirectionToDIrection(Normalize(Vector3{ 0.0f,0.0f,1.0f }), Normalize(move));
			directionMatrix_ = newDirection;
			//worldTransform_.matWorld_ *= newDirection;
			//worldTransform_.rotation_.y = newDirection.m[1][0] * newDirection.m[1][1] * newDirection.m[1][2];
			direction_ = move;
		}
		if (target_) {
			Vector3 toTarget = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
			toTarget.y = 0;
			directionMatrix_ = DirectionToDIrection(Normalize(Vector3{ 0.0f,0.0f,1.0f }), Normalize(toTarget));
		}
		worldTransform_.translation_ += move;
		if (worldTransform_.parent_ && particle_ && Length(move) != 0.0f) {
			Particle::ParticleData particleData;
			for (uint32_t count = 0; count < emitter.count; count++) {
				particleData.transform.scale = { 1.0f,1.0f,1.0f };
				particleData.transform.rotate = { 0.0f,0.0f,0.0f };
				particleData.transform.translate = worldTransform_.GetWorldPosition() + Vector3{ RandomEngine::GetRandom(-1.0f, 1.0f), RandomEngine::GetRandom(-1.0f, 1.0f), RandomEngine::GetRandom(-1.0f, 1.0f) };
				particleData.velocity = { RandomEngine::GetRandom(-1.0f,1.0f),RandomEngine::GetRandom(-1.0f,1.0f), RandomEngine::GetRandom(-1.0f,1.0f) };
				particleData.color = { 1.0f,1.0f,1.0f,1.0f };
				particleData.lifeTime = RandomEngine::GetRandom(1.0f, 3.0f);
				particleData.currentTime = 0;
				particle_->MakeNewParticle(particleData);
			}
		}
	}
	if (!worldTransform_.parent_) {
		velocity_ += kGravity;
	}
	worldTransform_.translation_ += velocity_;
	UpdateFloatingGimmick();
}

void Player::BehaviorAttackUpdate()
{
	static float weponRotateEnd = 3.14f;

	static float frontLength = 5.0f;
	/*
	if (frameCount_ < startFrame) {
		Vector3 move = {0.0f, 0.0f, frontLength / float(startFrame)};
		worldTransform_.translation_ +=
			Transform(move, MakeRotateMatrix(worldTransform_.rotation_));
	}*/
	if (frameCount_ >= attackFrame)
	{
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			//behavior_ = Behavior::kAttack;
			isCoenectCombo_ = true;
		}
	}
	if (frameCount_ == attackFrame)
	{
		attackBehavior_ = AttackBehavior::kAttack;
	}
	if (frameCount_ == rigidityFrame) {
		attackBehavior_ = AttackBehavior::kEnd;
	}
	if (frameCount_ == endFrame) {
		// behavior_ = Behavior::kRoot;
		weaponCollider_.SetIsCollision(false);
		if (isCoenectCombo_) {
			behaviorRequest_ = Behavior::kAttack2;
			comboNum_++;
		}
		else {
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	Vector3 move = { 0.0f, 0.0f, frontLength / float(attackFrame) };
	Matrix4x4 rotateMatrix = worldTransform_.matWorld_;
	rotateMatrix.m[3][0] = 0;
	rotateMatrix.m[3][1] = 0;
	rotateMatrix.m[3][2] = 0;

	if (target_) {
		Vector3 toTarget = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
		toTarget.y = 0;
		directionMatrix_ = DirectionToDIrection(Normalize(Vector3{ 0.0f,0.0f,1.0f }), Normalize(toTarget));
	}

	move = Transform(move, (rotateMatrix));

	switch (attackBehavior_) {
	case Player::AttackBehavior::kPre:
		if (target_) {
			if (Length(target_->GetWorldPosition() - worldTransform_.GetWorldPosition()) > 5.0f) {
				worldTransform_.translation_ += move;
			}
		}
		else {
			worldTransform_.translation_ += move;
		}
		break;
	case Player::AttackBehavior::kAttack:
		worldTransformWepon_.rotation_.x += weponRotateEnd / float(rigidityFrame - attackFrame);
		break;

	}

	worldTransformWepon_.UpdateMatrix();
	Vector3 weaponColliderCenter = worldTransformWepon_.GetWorldPosition();
	Vector3 offset{ 0, 5.0f, 0 };
	offset = TransformNormal(offset, worldTransformWepon_.GetRotate());
	weaponOBB_.center = weaponColliderCenter + offset;
	weaponOBB_.size = { 1.0f,2.5f,1.0f };
	SetOridentatios(weaponOBB_, worldTransformWepon_.matWorld_);
	weaponCollider_.SetOBB(weaponOBB_);
	worldTtansformOBB_.matWorld_ = MakeScaleMatrix(weaponOBB_.size) * worldTransformWepon_.GetRotate() * MakeTranslateMatrix(weaponOBB_.center);
	frameCount_++;
}

void Player::Draw(const ViewProjection& viewProjection) {
	//model_->Draw(worldTransform_, viewProjection);
	for (HierarchicalAnimation& model : models_)
	{
		model.model_->Draw(model.worldTransform_, viewProjection);
	}
	
}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;
}



void Player::OnCollision(WorldTransform& parent)
{
	if (worldTransform_.parent_ != &parent) {
		Matrix4x4 rocal = worldTransform_.matWorld_ * (Inverse(parent.matWorld_));
		worldTransform_.translation_.x = rocal.m[3][0];
		worldTransform_.translation_.y = rocal.m[3][1];
		worldTransform_.translation_.z = rocal.m[3][2];

		worldTransform_.parent_ = &parent;
		velocity_ = { 0,0,0 };
		isFlooar_ = true;
	}
}

