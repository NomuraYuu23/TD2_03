#include "SceneTransitionPitaverse.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/WinApp.h"

void SceneTransitionPitaverse::Initialize()
{

	ISceneTransition::Initialize();

	// 背景
	backgroundTextureHandle_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	backgroundColor_ = { 0.0f,0.0f,0.0f,0.0f };
	Vector2 position = { static_cast<float>(WinApp::kWindowWidth) / 2.0f, static_cast<float>(WinApp::kWindowHeight) / 2.0f };
	backgroundSprite_.reset(Sprite::Create(backgroundTextureHandle_, position, backgroundColor_));
	backgroundSprite_->SetSize(Vector2{ static_cast<float>(WinApp::kWindowWidth), static_cast<float>(WinApp::kWindowHeight) });

	// ロード文字
	loadTextureHandle_ = TextureManager::Load("Resources/default/load.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	loadColor_ = { 1.0f,1.0f,1.0f,0.0f };
	position = { 1100.0f, 600.0f };
	loadSprite_.reset(Sprite::Create(loadTextureHandle_, position, loadColor_));
	loadSprite_->SetSize({ 128.0f, 128.0f });
	loadSprite_->SetTextureSize({ 128.0f, 128.0f });
	loadSprite_->SetTextureLeftTop({ 0.0f, 0.0f });
	loadCount_ = 0;

}

void SceneTransitionPitaverse::Update()
{

	ISceneTransition::Update();

	// α値変更
	if (isFadeIn_) {
		float a = fadeTimer_ / fadeInTime_;
		backgroundColor_.w = a;
		loadColor_.w = a;
	}
	else {
		float a = 1.0f - fadeTimer_ / fadeOutTime_;
		backgroundColor_.w = a;
		loadColor_.w = a;
	}
	backgroundSprite_->SetColor(backgroundColor_);
	loadSprite_->SetColor(loadColor_);

	// ロード文字
	loadCount_ = loadCount_ + 2 % 80;
	loadSprite_->SetTextureLeftTop({ 128.0f * (static_cast<float>(loadCount_ / 10)), 0.0f });

}

void SceneTransitionPitaverse::Draw()
{

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommadList());

	backgroundSprite_->Draw();
	loadSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void SceneTransitionPitaverse::RegisteringGlobalVariables()
{
}

void SceneTransitionPitaverse::ApplyGlobalVariables()
{
}
