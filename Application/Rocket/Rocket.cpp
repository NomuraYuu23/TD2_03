#include "Rocket.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

void Rocket::Initialize(Model* model)
{

	model_ = model;

	worldTransform_.Initialize();

	toPlayerLength_ = 1.0f;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void Rocket::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

	worldTransform_.UpdateMatrix();

#endif // _DEBUG

}

void Rocket::Draw(BaseCamera camera)
{

	model_->Draw(worldTransform_, camera);

}

void Rocket::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Rocket";

	globalVariables->AddItem(groupName, "toPlayerLength", toPlayerLength_);


	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "rotate", worldTransform_.transform_.rotate);
	globalVariables->AddItem(groupName, "translate", worldTransform_.transform_.translate);


}

void Rocket::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Rocket";

	toPlayerLength_ = globalVariables->GetFloatValue(groupName, "toPlayerLength");

	worldTransform_.transform_.scale = globalVariables->GetVector3Value(groupName, "scale");
	worldTransform_.transform_.rotate = globalVariables->GetVector3Value(groupName, "rotate");
	worldTransform_.transform_.translate = globalVariables->GetVector3Value(groupName, "translate");

}
