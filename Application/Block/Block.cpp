#include "Block.h"
#include "UFO.h"
#include "../Player/player.h"
#include "../Collider/CollisionConfig.h"
void Block::Initialize() {
	anchorPoints_[0].position = {  0.0f, 1.0f, 0.0f };
	//anchorPoints_[1].position = { -0.8f, 1.0f, 0.8f };
	//anchorPoints_[2].position = {  0.8f, 1.0f,-0.8f };
	//anchorPoints_[3].position = { -0.8f, 1.0f,-0.8f };
	for (int index = 0; index < anchorNum; index++) {
		//anchorPoints_[index].isSting = false;
		anchorPoints_[index].screw = nullptr;
	}
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = {2.0f,2.0f,2.0f};

	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate,worldTransform_.rotateMatrix_,worldTransform_.transform_.scale,this);
	collider_->SetCollisionAttribute(kCollisionAttributeBlock);
	isConnect_ = false;
	isCenter_ = false;
	velocity_ = { 0,0,0};
	isRelese_ = false;
	isRidePlayer_ = false;
	mat_.reset(Material::Create());
	TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	mat_->Update(t, { 1.0f,1.0f,1.0f,1.0f }, 0, 200);
	whiteTextureHandle_ = 0;
	worldTransformOverRay_.Initialize();
	mat2_.reset(Material::Create());
	//TransformStructure t{ 0 };
	t.scale = { 1.0f,1.0f,1.0f };
	mat2_->Update(t, { 0.8f,0.5f,1.5f,1.0f }, 0, 200);
	worldTransformSoil_.Initialize();
	worldTransformSoil_.parent_ = &worldTransform_;
	isPinch_ = false;
	isPinchCheckMode_ = false;
	colorFrame_ = 0;
	colorLength_ = 15;
	colorDirection_ = 1;
}
void Block::Update() {
	reConnect_ = false;
	isRelese_ = false;
	bool isStack = false;
	for (int index = 0; index < anchorNum; index++) {
		if (anchorPoints_[index].screw != nullptr) {
			isStack = true;
			break;
		}
	}
	worldTransform_.transform_.translate.y = 0;
	if (!isStack && !isCenter_) {
		worldTransform_.transform_.translate.y = - collider_->size_.y * 2.0f;
	}
	if (!isConnect_) {
		worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate,velocity_);
	}
	if (isConnect_) {
		mat2_->SetColor({1.0f,0.4f,0.4f,1.0f});
		if (isPinch_) {
			float t = float(colorFrame_) / float(colorLength_);
			colorFrame_ += colorDirection_;
			if (colorFrame_>colorLength_){
				colorFrame_ = colorLength_;
				colorDirection_ = -1;
			}
			if (colorFrame_<0) {
				colorFrame_ = 0;
				colorDirection_ = 1;
			}
			Vector4 color;
			color.x = 1.0f;
			color.y = (1.0f - t) * 0.4f + t * 1.0f;
			color.z = (1.0f - t) * 0.4f + t * 1.0f;
			color.w = 1.0f;
			mat2_->SetColor(color);
		}
		else {
			colorFrame_ = 0;
			colorDirection_ = 1;
		}
		//bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr) {
				isStack = true;
				break;
			}
		}

		if (!isStack && !isCenter_) {
			isConnect_ = false;
			isRelese_ = true;
		}
	}
	else {
		mat2_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	worldTransform_.UpdateMatrix();
	worldTransformSoil_.transform_.translate.y = 1.5f;

	worldTransformSoil_.transform_.scale = {3.0f,3.0f,3.0f};
	worldTransformSoil_.UpdateMatrix();
	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->size_ = { worldTransform_.transform_.scale.x * 3.8f, worldTransform_.transform_.scale.y/2.0f, worldTransform_.transform_.scale.z * 3.8f };
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();

	if (isConnectAnimation_) {
		worldTransformOverRay_.transform_ = worldTransform_.transform_;
		worldTransformOverRay_.transform_.scale.x += 0.03f;
		worldTransformOverRay_.transform_.scale.y += 0.03f;
		worldTransformOverRay_.transform_.scale.z += 0.03f;
		worldTransformOverRay_.UpdateMatrix();
		mat_->SetColor({1.0f,1.0f,1.0f,alpha_});
		alpha_ -= 1.0f/30.0f;
		if (alpha_ < 0.0f) {
			isConnectAnimation_ = false;
		}
	}

	//isCenter_ = false;
	isRidePlayer_ = false;
	if (!isCenter_) {
		isPinch_ = true;
	}
}
void Block::Draw(Model* model, BaseCamera& camera) {
	model->Draw(worldTransform_,camera,mat2_.get());
	if (isConnectAnimation_) {
		model->Draw(worldTransformOverRay_, camera,mat_.get(), whiteTextureHandle_);
	}
}

void Block::DrawSoil(Model* model, BaseCamera& camera) {
	bool isStack = false;
	for (int index = 0; index < anchorNum; index++) {
		if (anchorPoints_[index].screw != nullptr) {
			isStack = true;
			break;
		}
	}
	if (isStack && !isCenter_) {
		model->Draw(worldTransformSoil_, camera);
	}
}

Vector3 Block::GetAnchorPointWorldPosition(size_t num) {
	return Matrix4x4Calc::Transform(anchorPoints_[num].position,worldTransform_.worldMatrix_);
}

void Block::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (!isConnect_ && std::holds_alternative<Magnet*>(pairObject)) {
		bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr && anchorPoints_[index].screw->GetState() == Screw::STUCK) {
				isStack = true;
				break;
			}
		}
		if (isStack) {
			Vector3 toPlayer = Vector3Calc::Subtract(std::get<Magnet*>(pairObject)->GetCenter(), worldTransform_.GetWorldPosition());
			toPlayer.y = 0;
			toPlayer = Vector3Calc::Normalize(toPlayer);
			toPlayer = Vector3Calc::Multiply(0.5f, toPlayer);
			worldTransform_.transform_.translate = Vector3Calc::Add(worldTransform_.transform_.translate, toPlayer);
		}
	}

	if (!isCenter_ && !isConnect_ && (std::holds_alternative<Block*>(pairObject) || (std::holds_alternative<UFO*>(pairObject)))) {
		bool isStack = false;
		for (int index = 0; index < anchorNum; index++) {
			if (anchorPoints_[index].screw != nullptr && anchorPoints_[index].screw->GetState() == Screw::STUCK) {
				isStack = true;
				break;
			}
		}
		if (isStack && std::get<Block*>(pairObject)->GetIsConnect()) {
			/*Matrix4x4 rocal = Matrix4x4Calc::Multiply(worldTransform_.worldMatrix_, (Matrix4x4Calc::Inverse(std::get<Block*>(pairObject)->GetWorldTransform()->worldMatrix_)));
			worldTransform_.transform_.translate.x = rocal.m[3][0];
			worldTransform_.transform_.translate.y = rocal.m[3][1];
			worldTransform_.transform_.translate.z = rocal.m[3][2];
			worldTransform_.p*/
			if (reConnect_ || isConnectableMode_) {
				isConnect_ = true;
				if (!reConnect_) {
					isConnectAnimation_ = true;
					mat_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
					alpha_ = 1.0f;
				}
			}
		}
	}
	if (std::holds_alternative<Player*>(pairObject)) {
		isRidePlayer_ = true;
	}
	if (isPinchCheckMode_) {
		if ((std::holds_alternative<Block*>(pairObject) || (std::holds_alternative<UFO*>(pairObject)))) {
			if (!std::get<Block*>(pairObject)->IsPinch()) {
				isPinch_ = false;
			}
		}
	}
}
