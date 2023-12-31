#include "UI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

void UI::Initialize(uint32_t textureHandle, const std::string& groupName)
{

	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	sprite_.reset(Sprite::Create(textureHandle, position, color));

	position_ = position;

	rotate_ = 0.0f;

	size_ = sprite_->GetSize();

	groupName_ = groupName;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void UI::Initialize(uint32_t textureHandle, const std::string& groupName, const Vector2& size, const Vector2& leftTop)
{

	UI::Initialize(textureHandle, groupName);

	sprite_->SetTextureSize(size);
	sprite_->SetTextureLeftTop(leftTop);

}

void UI::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

}

void UI::Update(const Vector2& leftTop)
{

	UI::Update();

	sprite_->SetTextureLeftTop(leftTop);

}

void UI::Draw()
{

	sprite_->Draw();

}

void UI::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->AddItem(groupName_, "position", position_);
	globalVariables->AddItem(groupName_, "rotate", rotate_);
	globalVariables->AddItem(groupName_, "size", size_);

}

void UI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	position_ = globalVariables->GetVector2Value(groupName_, "position");
	rotate_ = globalVariables->GetFloatValue(groupName_, "rotate");
	size_ = globalVariables->GetVector2Value(groupName_, "size");

	sprite_->SetPosition(position_);
	sprite_->SetRotate(rotate_);
	sprite_->SetSize(size_);

}
