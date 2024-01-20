#include "UIEnergy.h"

void UIEnergy::Initialize(uint32_t textureHandle, const std::string& groupName, const Vector2& size, const Vector2& leftTop, float energyMax)
{

	UI::Initialize(textureHandle, groupName, size, leftTop);

	energyMax_ = energyMax;

	UISizeMax_ = size_;

}

void UIEnergy::Update(float energy)
{

	UI::Update();

	// 割合
	float ratio = 1.0f;
	if (energy >= 0.0f && energy <= energyMax_) {
		ratio = energy / energyMax_;
	}

	Vector2 size = { UISizeMax_.x * ratio, UISizeMax_.y };

	sprite_->SetSize(size);

	Vector2 position = { position_.x - (UISizeMax_.x / 2) * (1.0f - ratio), position_.y };

	sprite_->SetPosition(position);

}
