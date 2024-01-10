#include "Player.h"
//#include "ImGuiManager.h"

#include <algorithm>
#include <cassert>
#include <numbers>

#include "../Engine/Math/Vector3.h"
#include "../Engine/Math/Matrix4x4.h"
#include "Block/Block.h"
#include "Screw.h"
//#include "GlobalVariables.h"

//#include "CollisionManager.h"
//#include "RandomEngine.h"

static int startFrame = 0;
static int endFrame = 40;
static int rigidityFrame = 30;
static int attackFrame = 15;


void Player::Initialize() {
	
	
	worldTransform_.Initialize();
	worldTransform_.transform_.translate.y += 4.0f;

	input_ = Input::GetInstance();
	direction_ = { 0,0,1.0f };
	directionMatrix_ = Matrix4x4Calc::MakeIdentity4x4();

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);

	magnet_.reset(new Magnet);
	magnet_->Initialize();
	mat_.reset(Material::Create());
	TransformStructure t{0};
	t.scale = {1.0f,1.0f,1.0f};
	mat_->Update(t, {0.8f,0.0f,0.0f,1.0f},0,200);
	materialCircle_.reset(Material::Create());
	//t.scale = {magnet_->GetRadius(),1.0f,magnet_->GetRadius() };
	materialCircle_->Update(t, { 0.8f,0.8f,0.8f,0.5f }, 0, 200);
	worldTransformCircle_.Initialize();
	worldTransformCircle_.transform_.scale = { magnet_->GetRadius(),0.5f,magnet_->GetRadius() };
}


void Player::BehaviorRootInitialize() {
	InitializeFloatingGimmick();
	
	//comboNum_ = 0;
}

void Player::BehaviorAttackInitialize() {

	velocity_ = { 0,0,0 };
	
}

void Player::BehaviorDropInitialize() {

	velocity_ = { 0,2.0f,0 };
	acceleration_ = {0,-0.15f,0};
	isFlooar_ = false;
}

void Player::Update(Block* block, size_t blockNum) {
	//ApplyGlobalVariables();
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
		case Player::Behavior::kDrop:
			BehaviorDropInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
	//BehaviorRootUpdate();
	//BehaviorAttackUpdate();
	/*if (worldTransform_.parent_) {
		if (worldTransform_.transform_.translate.y < 0.0f) {
			worldTransform_.transform_.translate.y = 0.0f;
		}

	}*/
	worldTransform_.transform_.translate.y -= 0.3f;
	if (worldTransform_.transform_.translate.y <= -20.0f) {
		behaviorRequest_ = Behavior::kRoot;
		worldTransform_.transform_.translate = {0.0f,4.0f,0.0f};
	}
	switch (behavior_) {
	case Player::Behavior::kRoot:
		BehaviorRootUpdate(block,blockNum);
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Player::Behavior::kDrop:
		BehaviorDropUpdate();
		break;
	}
	/*
	if (worldTransform_.translation_.y < -10.0f) {
		//ReStart();
	}*/

	// 行列更新
	//worldTransform_.UpdateMatrix();
	worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeScaleMatrix(worldTransform_.transform_.scale) , Matrix4x4Calc::Multiply( directionMatrix_ , Matrix4x4Calc::MakeTranslateMatrix(worldTransform_.transform_.translate)));
	if (worldTransform_.parent_) {
		worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_,worldTransform_.parent_->worldMatrix_);
	}
	//worldTransform_.UpdateMatrix();
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

	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();
	magnet_->SetCenter(worldTransform_.GetWorldPosition());
	magnet_->Update();

	worldTransformCircle_.transform_.translate = worldTransform_.GetWorldPosition();
	worldTransformCircle_.UpdateMatrix();

	isFlooar_ = false;
	//preJoyState_ = joyState_;
}

void Player::BehaviorRootUpdate(Block* block, size_t blockNum)
{
	// ゲームパッドの状態をえる
	input_ = Input::GetInstance();
	if (input_->TriggerJoystick(5) && block)
	{
		//behavior_ = Behavior::kAttack;
		
		Screw* anchorScrew = block->GetAnchorPointScrew(blockNum);
		//刺す
		if (!anchorScrew ) {
			for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_->begin(); ite != screws_->end();ite++) {
				if ((*ite)->GetState() == Screw::FOLLOW) {
					(*ite)->Throw(worldTransform_.GetWorldPosition(),block,blockNum);
					behaviorRequest_ = Behavior::kAttack;
					break;
				}
			}
			
		}
		//抜く
		else if (anchorScrew) {
			anchorScrew->Catch();
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA) && isFlooar_)
	{
		behaviorRequest_ = Behavior::kDrop;
	}

	if (1) {

		const float kCharacterSpeed = 0.3f;

		Vector3 move = {
			float(input_->GetLeftAnalogstick().x) / (SHRT_MAX), 0,
			float(-input_->GetLeftAnalogstick().y) / (SHRT_MAX) };
		//Matrix4x4 newrotation = DirectionToDIrection({0,0.0f,1.0f}, {0, 0.0f, -1.0f});
		move = Vector3Calc::Multiply(kCharacterSpeed, Vector3Calc::Normalize(move));;
		//Vector3 cameraDirectionYcorection = {0.0f, viewProjection_->matView.m[1][0] * viewProjection_->matView.m[1][0]* viewProjection_->matView.m[1][2], 0.0f};
		Matrix4x4 cameraRotateY = Matrix4x4Calc::Inverse(camera_->GetViewMatrix());
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
		move = Matrix4x4Calc::Transform(move, cameraRotateY);
		if (input_->GetLeftAnalogstick().x != 0 || input_->GetLeftAnalogstick().y != 0) {
			//worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			Matrix4x4 newDirection = Matrix4x4Calc::DirectionToDirection(Vector3Calc::Normalize(Vector3{ 0.0f,0.0f,1.0f }), Vector3Calc::Normalize(move));
			directionMatrix_ = newDirection;
			//worldTransform_.matWorld_ *= newDirection;
			//worldTransform_.rotation_.y = newDirection.m[1][0] * newDirection.m[1][1] * newDirection.m[1][2];
			direction_ = move;
		}
		if (target_) {
			//Vector3 toTarget = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
			Vector3 toTarget = Vector3Calc::Subtract(target_->GetWorldPosition(), worldTransform_.GetWorldPosition());
			toTarget.y = 0;
			directionMatrix_ = Matrix4x4Calc::DirectionToDirection(Vector3Calc::Normalize(Vector3{ 0.0f,0.0f,1.0f }), Vector3Calc::Normalize(toTarget));
		}
		//worldTransform_.transform_.translate += move;
		worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,move);
	}
	if (!worldTransform_.parent_) {
		//velocity_ = Vector3Calc::Add( velocity_,kGravity);
	}
	//worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,velocity_);
	
}

void Player::BehaviorAttackUpdate()
{
	if (frameCount_ >15) {
		behaviorRequest_ = Behavior::kRoot;
	}

	frameCount_++;
}

void Player::BehaviorDropUpdate()
{
	velocity_ = Vector3Calc::Add(velocity_, acceleration_);
	worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity_);
	if (isFlooar_) {
		//反転処理
		for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_->begin(); ite != screws_->end(); ite++) {
			float distance = Vector3Calc::Length(Vector3Calc::Subtract(worldTransform_.GetWorldPosition(),(*ite)->GetWorldTransform()->GetWorldPosition()));
			if (distance <= magnet_->GetRadius()) {
				(*ite)->TurnOver();
			}
		}

		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_, camera,mat_.get());
	modelCircle_->Draw(worldTransformCircle_, camera, materialCircle_.get());
}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;
}



void Player::OnCollision(ColliderParentObject pairObject, CollisionData collidionData)
{
	if (std::holds_alternative<Block*>(pairObject)) {
		/*if (worldTransform_.parent_ != std::get<Block*>(pairObject)->GetWorldTransform()) {
			Matrix4x4 rocal = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_, (Matrix4x4Calc::Inverse(std::get<Block*>(pairObject)->GetWorldTransform()->worldMatrix_)));
			worldTransform_.transform_.translate.x = rocal.m[3][0];
			worldTransform_.transform_.translate.y = rocal.m[3][1];
			worldTransform_.transform_.translate.z = rocal.m[3][2];


			Vector3 axis;
			axis = { rocal.m[0][0],rocal.m[0][1],rocal.m[0][2] };
			float length = Vector3Calc::Length(axis);
			worldTransform_.transform_.scale.x = length;
			axis = { rocal.m[1][0],rocal.m[1][1],rocal.m[1][2] };
			worldTransform_.transform_.scale.y = length;
			axis = { rocal.m[2][0],rocal.m[2][1],rocal.m[2][2] };
			worldTransform_.transform_.scale.z = length;

			worldTransform_.parent_ = std::get<Block*>(pairObject)->GetWorldTransform();
			velocity_ = { 0,0,0 };
			isFlooar_ = true;
		}*/
		isFlooar_ = true;
		worldTransform_.transform_.translate.y = std::get<Block*>(pairObject)->GetWorldTransform()->GetWorldPosition().y + std::get<Block*>(pairObject)->GetWorldTransform()->transform_.scale.y + worldTransform_.transform_.scale.y;
		worldTransform_.UpdateMatrix();
		//std::get<Block*>(pairObject)->SetIsCenter(true);
		//std::get<Block*>(pairObject)->SetIsConnect(true);
	}
}

