#include "UFOManager.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
void UFOManager::Initialize() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "UFO";

	globalVariables->AddItem(groupName, "SpawnInterval", spawnInterval_);
	timer_ = 0;
	frameCount_ = 0;
}

void UFOManager::Update() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "UFO";
	spawnInterval_ = globalVariables->GetIntValue(groupName, "SpawnInterval");

	frameCount_++;
	if (frameCount_ >=60) {
		frameCount_ = 0;
		timer_++;
	}
	if (timer_ >= spawnInterval_) {
		timer_ = 0;
		std::unique_ptr<UFO> ufo_;
		ufo_.reset(new UFO);
		ufo_->Initialize();
		ufo_->SetCircle(modelCircle_);
		ufo_->SetWorldPosition({ -10.0f,12.0f,150.0f });
		ufo_->SetVelocity({ 0.0f,0.0f,-0.1f });
		ufos_->push_back(std::move(ufo_));
	}
}
