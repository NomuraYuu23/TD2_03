#include "Rocket.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

void Rocket::Initialize(Model* model, Model* soilModel)
{

	model_ = model;

	worldTransform_.Initialize();

	toPlayerLength_ = 1.0f;

	soilModel_ = soilModel;

	soilWorldTransform_.Initialize();

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

	worldTransform_.UpdateMatrix();

	soilWorldTransform_.UpdateMatrix();

}

void Rocket::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();

	worldTransform_.UpdateMatrix();
	soilWorldTransform_.UpdateMatrix();

#endif // _DEBUG

}

void Rocket::Draw(BaseCamera camera)
{

	model_->Draw(worldTransform_, camera);

	soilModel_->Draw(soilWorldTransform_, camera);

}

void Rocket::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Rocket";

	globalVariables->AddItem(groupName, "toPlayerLength", toPlayerLength_);


	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "rotate", worldTransform_.transform_.rotate);
	globalVariables->AddItem(groupName, "translate", worldTransform_.transform_.translate);

	globalVariables->AddItem(groupName, "soilScale", soilWorldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "soilRotate", soilWorldTransform_.transform_.rotate);
	globalVariables->AddItem(groupName, "soilTranslate", soilWorldTransform_.transform_.translate);


}

void Rocket::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Rocket";

	toPlayerLength_ = globalVariables->GetFloatValue(groupName, "toPlayerLength");

	worldTransform_.transform_.scale = globalVariables->GetVector3Value(groupName, "scale");
	worldTransform_.transform_.rotate = globalVariables->GetVector3Value(groupName, "rotate");
	worldTransform_.transform_.translate = globalVariables->GetVector3Value(groupName, "translate");
	
	soilWorldTransform_.transform_.scale = globalVariables->GetVector3Value(groupName, "soilScale");
	soilWorldTransform_.transform_.rotate = globalVariables->GetVector3Value(groupName, "soilRotate");
	soilWorldTransform_.transform_.translate = globalVariables->GetVector3Value(groupName, "soilTranslate");

}
