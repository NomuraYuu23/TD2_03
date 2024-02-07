#include "Player.h"
//#include "ImGuiManager.h"

#include <algorithm>
#include <cassert>
#include <numbers>

#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Matrix4x4.h"
#include "../Block/Block.h"
#include "../Screw/Screw.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../Particle/EmitterName.h"
#include "../Planet/Planet.h"
//#include "GlobalVariables.h"

//#include "CollisionManager.h"
//#include "RandomEngine.h"

static int startFrame = 0;
static int endFrame = 40;
static int rigidityFrame = 30;
static int attackFrame = 15;


void Player::Initialize(const std::array<std::unique_ptr<Model>, PlayerPartIndex::kPlayerPartIndexOfCount>& models) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Player";

	globalVariables->AddItem(groupName, "MagnetRadius", magnetRadius_);
	globalVariables->AddItem(groupName, "GravityFrame", gravityFrame_);
	globalVariables->AddItem(groupName, "CharacterSpeed", characterSpeed_);
	globalVariables->AddItem(groupName, "NotFallLength", notFallLength_);
	globalVariables->AddItem(groupName, "SizeUpTime", sizeUpLength_);
	globalVariables->AddItem(groupName, "MagnetBigRadius", magnetRadius_);
	magnetRadius_ = globalVariables->GetFloatValue(groupName, "MagnetRadius");
	globalVariables->AddItem(groupName, "ColorChangeSpeed", colorChangeSpeed_);
	globalVariables->AddItem(groupName, "WhiteWeight", clampdWhiteWait_);
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
	mat_->Update(t, {1.0f,1.0f,1.0f,1.0f},0,200);
	materialCircle_.reset(Material::Create());
	//t.scale = {magnet_->GetRadius(),1.0f,magnet_->GetRadius() };
	materialCircle_->Update(t, { 0.8f,0.8f,0.8f,0.5f }, 0, 200);
	worldTransformCircle_.Initialize();
	worldTransformCircle_.transform_.scale = { magnet_->GetRadius(),0.5f,magnet_->GetRadius() };

	for (uint32_t i = 0; i < models_.size(); i++) {
		models_[i] = models[i].get();
	}

	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->Initialize(this);

	playerAnimationNo_ = kPlayerAnimationIndexStand;
	controlLength_ = 0;
	isCanShot_ = true;
	isCanGravity_ = true;
	isUsedGravity_ = false;
	isCanLockOn_ = true;
	sizeUpTime_ = 0;
	magnetRadiusNow_ = 24.0f;
}


void Player::BehaviorRootInitialize() {
	InitializeFloatingGimmick();
	
	//comboNum_ = 0;
}

void Player::BehaviorAttackInitialize() {

	velocity_ = { 0,0,0 };
	playerAnimationNo_ = kPlayerAnimationIndexScrewThrowing;

	if (block_) {
		Vector3 blockPosition = block_->GetAnchorPointWorldPosition(0);
		blockPosition.y = 0.0f;
		Vector3 playerPosition = worldTransform_.GetWorldPosition();
		playerPosition.y = 0.0f;
		direction_ = 
			Vector3Calc::Normalize(Vector3Calc::Subtract(blockPosition, playerPosition));
	}
	
}

void Player::BehaviorDropInitialize() {

	velocity_ = { 0,2.0f,0 };
	acceleration_ = {0,-0.15f,0};
	//isFlooar_ = false;
	playerAnimationNo_ = kPlayerAnimationIndexGravity;
	audioManager_->PlayWave(kGameAudioNameIndexPlayerGravity);

}

void Player::Update(Block* block, size_t blockNum) {
	//ApplyGlobalVariables();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Player";
	magnetRadius_ = globalVariables->GetFloatValue(groupName, "MagnetRadius");
	sizeUpLength_ = globalVariables->GetIntValue(groupName, "SizeUpTime");
	colorChangeSpeed_ = globalVariables->GetFloatValue(groupName, "ColorChangeSpeed");
	clampdWhiteWait_ = globalVariables->GetFloatValue(groupName, "WhiteWeight");
	if (sizeUpTime_>0) {
		RainbowColor();
		sizeUpTime_--;
		//magnetRadius_ = 48.0f;
		magnetRadius_ = globalVariables->GetFloatValue(groupName, "MagnetBigRadius");
	}
	else {
		materialCircle_->SetColor({ 0.8f,0.8f,0.8f,0.5f });
	}
	if (std::abs(magnetRadius_ - magnetRadiusNow_) > 2.1f) {
		if (magnetRadius_ > magnetRadiusNow_) {
			magnetRadiusNow_ += 2.0f;
		}
		else {
			magnetRadiusNow_ -= 2.0f;
		}
	}
	else {
		magnetRadiusNow_ = magnetRadius_;
	}
	gravityFrame_ = globalVariables->GetUIntValue(groupName, "GravityFrame");
	characterSpeed_ = globalVariables->GetFloatValue(groupName, "CharacterSpeed");
	notFallLength_ = globalVariables->GetIntValue(groupName, "NotFallLength");
	if (isRideConnectFlooar_) {
		prePosition_ = worldTransform_.transform_.translate;
	}
	if (isLastBlockConnect_ && !isFlooar_ && notFallTime_>0) {
		//playerAnimationNo_ = kPlayerAnimationIndexFalling;
		worldTransform_.transform_.translate = prePosition_;
		//worldTransform_.transform_.translate.z = prePosition_.z;
		isFlooar_ = true;
		isRideConnectFlooar_ = true;
		if (behavior_ == Behavior::kRoot) {
			notFallTime_--;
			isNotFall = true;
		}
		else {
			isNotFall = false;
		}
	}
	else {
		isNotFall = false;
	}
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		attackFrameCount_ = 0;
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
	if (worldTransform_.transform_.translate.y <= -20.0f || isOutGameArea()) {
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

	// ロケット判定
	CollisionWithRocket();

	// 行列更新
	//worldTransform_.UpdateMatrix();
	worldTransform_.direction_ = direction_;
	worldTransform_.usedDirection_ = true;

	//worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeScaleMatrix(worldTransform_.transform_.scale) , Matrix4x4Calc::Multiply( directionMatrix_ , Matrix4x4Calc::MakeTranslateMatrix(worldTransform_.transform_.translate)));
	//if (worldTransform_.parent_) {
	//	worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_,worldTransform_.parent_->worldMatrix_);
	//}
	worldTransform_.UpdateMatrix();
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

	playerAnimation_->Update(playerAnimationNo_);

	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->center_.y -= 0.1f;
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();
	magnet_->SetCenter(worldTransform_.GetWorldPosition());
	magnet_->SetRadius(magnetRadiusNow_);
	magnet_->Update();
	worldTransformCircle_.transform_.scale = { magnet_->GetRadius(),0.5f,magnet_->GetRadius() };
	worldTransformCircle_.transform_.translate = worldTransform_.GetWorldPosition();
	worldTransformCircle_.UpdateMatrix();

	isFlooar_ = false;
	//isRideConnectFlooar_ = false;
	//preJoyState_ = joyState_;
}

void Player::BehaviorRootUpdate(Block* block, size_t blockNum)
{

	// ブロック情報
	if (block_) {
		block_->SetPlayerHasBeenSeen(false);
	}
	block_ = block;
	if (block_) {
		block_->SetPlayerHasBeenSeen(true);
	}

	// ゲームパッドの状態をえる
	input_ = Input::GetInstance();
	if (input_->TriggerJoystick(5) && block_ && isFlooar_)
	{
		//behavior_ = Behavior::kAttack;
		
		Screw* anchorScrew = block_->GetAnchorPointScrew(blockNum);
		//刺す
		if (!anchorScrew && isCanShot_) {
			bool isThrow=false;
			for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_->begin(); ite != screws_->end();ite++) {
				if ((*ite)->GetState() == Screw::FOLLOW) {
					//(*ite)->Throw(worldTransform_.GetWorldPosition(), block_,blockNum);
					WorldTransform* magnetWorldTransform = &((*playerAnimation_->GetWorldTransforms())[PlayerPartIndex::kPlayerPartIndexMagnet]);
					(*ite)->Catch(magnetWorldTransform);
					holdScrew_ = (*ite).get();
					blockNum_ = blockNum;
					behaviorRequest_ = Behavior::kAttack;
					isThrow = true;
					break;
				}
			}
			//追従が一体もいなかったとき
			if (!isThrow) {
				Screw* screw=nullptr;
				int length = -1;
				for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_->begin(); ite != screws_->end(); ite++) {
					if ((*ite)->GetState() == Screw::STUCK && (*ite)->GetStackLength()>length) {
						length = (*ite)->GetStackLength();
						screw = (*ite).get();
					}
				}
				if (screw) {
					//(*ite)->Throw(worldTransform_.GetWorldPosition(), block_,blockNum);
					WorldTransform* magnetWorldTransform = &((*playerAnimation_->GetWorldTransforms())[PlayerPartIndex::kPlayerPartIndexMagnet]);
					screw->GetTarget()->SetAnchorPointScrew(0, nullptr);
					screw->Catch(magnetWorldTransform);
					holdScrew_ = screw;
					blockNum_ = blockNum;
					behaviorRequest_ = Behavior::kAttack;
					isThrow = true;
				}
			}
		}
	}

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA) && isFlooar_ && isCanGravity_)
	{
		isUsedGravity_ = true;
		behaviorRequest_ = Behavior::kDrop;
		TransformStructure transform{ {1.0f,1.0f,1.0f},{0},{0.0f,3.0f,0.0f} };
		transform.translate = worldTransformCircle_.GetWorldPosition();
		ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.005f, 0.5f, ParticleModelIndex::kCircle, ParticleName::kGravityParticle, EmitterName::kGravityEmitter);
	}

	if (isFlooar_) {

		Vector3 move = {
			float(input_->GetLeftAnalogstick().x) / (SHRT_MAX), 0,
			float(-input_->GetLeftAnalogstick().y) / (SHRT_MAX) };
		if (!(move.x==0 && move.z == 0)) {
			controlLength_++;
		}
		//Matrix4x4 newrotation = DirectionToDIrection({0,0.0f,1.0f}, {0, 0.0f, -1.0f});
		move = Vector3Calc::Multiply(characterSpeed_, Vector3Calc::Normalize(move));;
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

		bool islockOn = false;
		if (target_ != nullptr) {
			//Vector3 toTarget = target_->GetWorldPosition() - worldTransform_.GetWorldPosition();
			Vector3 toTarget = Vector3Calc::Subtract(target_->GetWorldPosition(), worldTransform_.GetWorldPosition());
			toTarget.y = 0;
			direction_ = toTarget;
			directionMatrix_ = Matrix4x4Calc::DirectionToDirection(Vector3Calc::Normalize(Vector3{ 0.0f,0.0f,1.0f }), Vector3Calc::Normalize(toTarget));
			move = { 0.0f , 0.0f , 0.0f };
			islockOn = true;
		}
		else if (input_->PushJoystick(JoystickButton::kJoystickButtonLB) && isCanLockOn_) {
			move = { 0.0f , 0.0f , 0.0f };
			islockOn = true;
		}
		else {
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, move);
		}
		// アニメーション
		if (isNotFall) {
			playerAnimationNo_ = kPlayerAnimationIndexLikelyToFall;
		}
		else if (islockOn) {
			playerAnimationNo_ = kPlayerAnimationIndexLockOn;
		}
		else if (Vector3Calc::Length(move) == 0.0f) {
			playerAnimationNo_ = kPlayerAnimationIndexStand;
		}
		else {
			playerAnimationNo_ = kPlayerAnimationIndexWalk;
		}

	}
	else {
		playerAnimationNo_ = kPlayerAnimationIndexFalling;
		//worldTransform_.transform_.translate = prePosition_;
	}
	if (!worldTransform_.parent_) {
		//velocity_ = Vector3Calc::Add( velocity_,kGravity);
	}
	//worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,velocity_);
	
}

void Player::BehaviorAttackUpdate()
{
	if (attackFrameCount_ > attackFrame_) {
		behaviorRequest_ = Behavior::kRoot;
	}

	if (playerAnimation_->GetScrewThrowPhase() == 3 && holdScrew_) {
		holdScrew_->Throw(worldTransform_.GetWorldPosition(),block_,blockNum_);
	}

	// ねじさすタイミング
	if (attackFrameCount_ == 20) {
		audioManager_->PlayWave(kGameAudioNameIndexScrewShoot);
		TransformStructure transform{ {1.0f,1.0f,1.0f},{0},{0.0f,0.0f,0.0f} };
		transform.translate = holdScrew_->GetWorldTransform()->GetWorldPosition();
		transform.translate.y -= 5.0f;
		ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.002f, 0.2f, ParticleModelIndex::kCircle, ParticleName::kSparkParticle, EmitterName::kSparkEmitter);
	}

	attackFrameCount_++;

	worldTransform_.transform_.rotate.z = 0.0f;
}

void Player::BehaviorDropUpdate()
{
	//velocity_ = Vector3Calc::Add(velocity_, acceleration_);
	//worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, velocity_);
	if (gravityFrameCount_ == 0) {
		//反転処理
		for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_->begin(); ite != screws_->end(); ite++) {
			float distance = Vector3Calc::Length(Vector3Calc::Subtract(worldTransform_.GetWorldPosition(),(*ite)->GetWorldTransform()->GetWorldPosition()));
			if (distance <= magnet_->GetRadius()) {
				(*ite)->TurnOver();
			}
		}
		//behaviorRequest_ = Behavior::kRoot;
	}
	if (gravityFrameCount_++ == gravityFrame_) {
		gravityFrameCount_ = 0;
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::Draw(Model* model, BaseCamera& camera) {
	//model->Draw(worldTransform_, camera,mat_.get());
	model;
	playerAnimation_->Draw(models_, camera, mat_.get());

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
		isLastBlockConnect_ = std::get<Block*>(pairObject)->GetIsConnect();
		if (std::get<Block*>(pairObject)->GetIsConnect()) {
			isRideConnectFlooar_ = true;
			notFallTime_ = notFallLength_;
		}
		float sizeY = std::get<Block*>(pairObject)->GetCollider()->size_.y;
		worldTransform_.transform_.translate.y = std::get<Block*>(pairObject)->GetWorldTransform()->GetWorldPosition().y + sizeY + worldTransform_.transform_.scale.y;
		//worldTransform_.transform_.translate.y -= 0.001f;
		worldTransform_.UpdateMatrix();
		// worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeScaleMatrix(worldTransform_.transform_.scale) , Matrix4x4Calc::Multiply( directionMatrix_ , Matrix4x4Calc::MakeTranslateMatrix(worldTransform_.transform_.translate)));
		if (worldTransform_.parent_) {
			//worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_, worldTransform_.parent_->worldMatrix_);
		}
		//std::get<Block*>(pairObject)->SetIsCenter(true);
		//std::get<Block*>(pairObject)->SetIsConnect(true);
	}
	if (std::holds_alternative<Planet*>(pairObject)) {

		if (worldTransform_.transform_.translate.y >= 0.0f) {
			isFlooar_ = true;
			isLastBlockConnect_ = false;
			float sizeY = std::get<Planet*>(pairObject)->GetCollider()->size_.y;
			worldTransform_.transform_.translate.y = std::get<Planet*>(pairObject)->GetWorldTransform()->GetWorldPosition().y + sizeY + worldTransform_.transform_.scale.y;
			if (worldTransform_.parent_) {
				worldTransform_.worldMatrix_ = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_, worldTransform_.parent_->worldMatrix_);
			}
		}
		
	}

}

bool Player::isOutGameArea() {
	float length = std::sqrtf(std::powf(worldTransform_.GetWorldPosition().x,2) + std::powf(worldTransform_.GetWorldPosition().z, 2));
	if (length >= 190.0f) {
		isLastBlockConnect_ = false;
		return true;
	}
	return false;
}

void Player::CollisionWithRocket()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	Vector3 playerPos = worldTransform_.GetWorldPosition();
	float playerPosY = playerPos.y;
	playerPos.y = 0.0f;

	Vector3 rocketPos = rocket_->GetWorldTransform()->GetWorldPosition();
	rocketPos.y = 0.0f;

	Vector3 toPlayer = v3Calc->Subtract(playerPos, rocketPos);

	float length = v3Calc->Length(toPlayer);

	if (length < rocket_->GetToPlayerLength()) {
		Vector3 dirction = {};
		if (length == 0.0f) {
			dirction = {0.0f,0.0f,-1.0f};
		}
		else {
			dirction = v3Calc->Normalize(toPlayer);
		}
		Vector3 move = v3Calc->Multiply(rocket_->GetToPlayerLength() + 0.0001f, dirction);

		worldTransform_.transform_.translate = v3Calc->Add(rocketPos, move);
		worldTransform_.transform_.translate.y = playerPosY;
	}

}

void Player::RainbowColor() {
	static int colorPhase_ = 0;
	static float color_ = 0.0f;
	Vector3 color = {0};
	switch (colorPhase_)
	{
	case 0:
		color = Vector3{ 1.0f - color_,color_, 0.0f };
		break;
	case 1:
		color = Vector3{ 0.0f,1.0f - color_, color_ };
		break;
	case 2:
		color = Vector3{ color_,0.0f, 1.0f - color_ };
		break;
	default:
		break;
	}
	color = Vector3Calc::Multiply(1.0f-clampdWhiteWait_, color);
	color = Vector3Calc::Add(color, Vector3Calc::Multiply(clampdWhiteWait_,Vector3{ 1.0f,1.0f,1.0f }));
	color_ += colorChangeSpeed_;
	if (color_ > 1.0f) {
		color_ = 0;
		colorPhase_++;
		if (colorPhase_ > 2) {
			colorPhase_ = 0;
		}
	}
	Vector4 colorv4 = { color.x,color.y,color.z, 0.5f };
	materialCircle_->SetColor(colorv4);
}

void Player::StartUpdate()
{

	playerAnimation_->Update(playerAnimationNo_);

}
