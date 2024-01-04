#include "Planet.h"
#include <cassert>
#include "../../Engine/2D/ImguiManager.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

void Planet::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	// ワールド変換データの初期化
	worldTransform_.Initialize();

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void Planet::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

#endif // _DEBUG


	worldTransform_.transform_.scale = { size_, size_, size_ };
	worldTransform_.transform_.rotate.y = fmodf(worldTransform_.transform_.rotate.y + rotateSpeed_, 6.24f);
	worldTransform_.transform_.translate = position_;

	worldTransform_.UpdateMatrix();

}

void Planet::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera, material_.get());

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
