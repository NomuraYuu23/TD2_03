#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	titleTextureHandle_ = TextureManager::Load("Resources/Title/title.png", dxCommon_, textureHandleManager_.get()),
	titleSprite_.reset(Sprite::Create(titleTextureHandle_, Vector2{ 640.0f, 360.0f }, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));

	buttonTextureHandle_ = TextureManager::Load("Resources/Title/controler_UI_A.png", dxCommon_, textureHandleManager_.get()),
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, Vector2{ 640.0f, 560.0f }, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonSprite_->SetTextureSize(Vector2{ 384.0f, 384.0f });
	buttonSprite_->SetSize(Vector2{ 128.0f, 128.0f });
	buttonSprite_->SetTextureLeftTop(Vector2{0.0f, 0.0f});

	audioManager_ = std::make_unique<TitleAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();
	audioManager_->PlayWave(kTitleAudioNameIndexBGM);

}

void TitleScene::Update()
{

	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) &&
		requestSceneNo == kTitle) {
		// 行きたいシーンへ
		requestSceneNo = kGame;
		audioManager_->PlayWave(kTitleAudioNameIndexDecision);
	}

	// BGM音量下げる
	if (requestSceneNo == kGame && isDecreasingVolume) {
		LowerVolumeBGM();
	}
}

void TitleScene::Draw()
{

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画
	titleSprite_->Draw();
	buttonSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TitleScene::ModelCreate()
{
}

void TitleScene::MaterialCreate()
{
}

void TitleScene::TextureLoad()
{
}

void TitleScene::LowerVolumeBGM()
{

	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kTitleAudioNameIndexBGM) {
			float decreasingVolume = 1.0f / 60.0f;
			float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
			if (volume < 0.0f) {
				volume = 0.0f;
				audioManager_->StopWave(i);
				isDecreasingVolume = false;
			}
			else {
				audioManager_->SetPlayingSoundDataVolume(i, volume);
				audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
			}
		}
	}

}
