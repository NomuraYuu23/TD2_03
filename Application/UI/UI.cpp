#include "UI.h"

void UI::Initialize(uint32_t textureHandle)
{

	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	sprite_.reset(Sprite::Create(textureHandle, position, color));

	position_ = position;

	rotate_ = 0.0f;

	size_ = sprite_->GetSize();

}

void UI::Draw()
{

	sprite_->Draw();

}
