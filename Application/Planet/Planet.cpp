#include "Planet.h"
#include <cassert>
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/Math/Ease.h"
#include "../Particle/EmitterName.h"

void Planet::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransformFlag_.Initialize();
	collider_.reset(new OBB);
	collider_->Initialize(worldTransform_.transform_.translate, worldTransform_.rotateMatrix_, worldTransform_.transform_.scale, this);
	//collider_->SetCollisionAttribute(kCollisionAttributeBlock);

	flagLifeTime_ = 0;

	RegisteringGlobalVariables();

	//ApplyGlobalVariables();

}

void Planet::Update()
{

#ifdef _DEBUG

	//ApplyGlobalVariables();

#endif // _DEBUG

	//worldTransform_.transform_.scale = { size_, size_, size_ };
	//worldTransform_.transform_.rotate.y = fmodf(worldTransform_.transform_.rotate.y + rotateSpeed_, 6.24f);
	worldTransform_.transform_.translate = position_;
	worldTransform_.transform_.translate.y =- collider_->size_.y + 0.5f;
	worldTransformFlag_.transform_.translate = position_;
	worldTransformFlag_.transform_.translate.y = 4.0f;

	if (flagLifeTime_ > 0) {
		flagLifeTime_--;
		float t = static_cast<float>(flagLifeTime_) / static_cast<float>(flagrRmainingLife);
		worldTransformFlag_.transform_.translate.y = Ease::Easing(Ease::EaseName::EaseOutCirc, 16.0f, 4.0f, t);
		if (flagLifeTime_ == 0) {
			TransformStructure transformStructure = { 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 0.0f,
				Vector3{worldTransformFlag_.transform_.translate} };
			ParticleManager::GetInstance()->MakeEmitter(transformStructure, 10, 0.005f, 0.1f, kStar, kFlagClearParticle, kFlagClearEmitter);
		}
	}

	worldTransform_.UpdateMatrix();
	worldTransformFlag_.UpdateMatrix();

	collider_->center_ = worldTransform_.GetWorldPosition();
	collider_->size_ = { worldTransform_.transform_.scale.x * 8.0f, worldTransform_.transform_.scale.y * 8.0f, worldTransform_.transform_.scale.z * 8.0f };
	collider_->SetOtientatuons(worldTransform_.rotateMatrix_);
	collider_->worldTransformUpdate();

}

void Planet::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera, material_.get());

}

void Planet::DrawFlag(BaseCamera& camera)
{

	modelFlag_->Draw(worldTransformFlag_, camera, material_.get(), flagTextureHandle_);

}

void Planet::ImGuiDraw()
{

	ImGui::Begin("Planet");
	ImGui::DragFloat("rotateSpeed", &rotateSpeed_, 0.001f);
	ImGui::DragFloat3("position", &position_.x, 0.1f);
	ImGui::DragFloat("size", &size_, 0.001f);
	ImGui::End();

}

void Planet::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Planet";

	globalVariables->AddItem(groupName, "rotateSpeed", rotateSpeed_);
	globalVariables->AddItem(groupName, "position", position_);
	globalVariables->AddItem(groupName, "size", size_);

}

void Planet::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Planet";

	position_ = globalVariables->GetVector3Value(groupName, "position");
	rotateSpeed_ = globalVariables->GetFloatValue(groupName, "rotateSpeed");
	size_ = globalVariables->GetFloatValue(groupName, "size");

}
