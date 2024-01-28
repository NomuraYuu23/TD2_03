#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	titlePosition_ = { 320.0f, 240.0f };
	titleSprite_.reset(Sprite::Create(titleTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	titleSize_ = titleSprite_->GetSize();

	buttonPosition_ = { 640.0f, 560.0f };
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, buttonPosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonSprite_->SetTextureSize(Vector2{ 384.0f, 384.0f });
	buttonSprite_->SetSize(Vector2{ 128.0f, 128.0f });
	buttonSprite_->SetTextureLeftTop(Vector2{0.0f, 0.0f});
	buttonSize_ = { 128.0f, 128.0f };
	buttonAlphaT_ = 0.0f;
	buttonAlphaTSpeed_ = 0.01f;
	buttonItIncreaseAlphaT_ = true;
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	audioManager_ = std::make_unique<TitleAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();
	audioManager_->PlayWave(kTitleAudioNameIndexBGM);

	// ビュープロジェクション
	TransformStructure baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.0f,0.0f,0.0f,
		0.0f, 0.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// プレイヤー
	player_ = std::make_unique<TitleObj>();
	player_->Initialize(playerModel_.get(), "Player");

	// ねじ
	for (uint32_t i = 0; i < screws_.size(); ++i) {
		screws_[i] = std::make_unique<TitleObj>();
		screws_[i]->Initialize(screwModel_.get(), "Screw" + std::to_string(i));
	}

	SpriteRegisteringGlobalVariables();

	SpriteApplyGlobalVariables();

	outline_.Initialize();
	outline_.color_ = { 0.8f,0.4f,0.1f,1.0f };
	//アウトライン
	outline_.Map();
}

void TitleScene::Update()
{

#ifdef _DEBUG
	SpriteApplyGlobalVariables();
#endif // _DEBUG


	ImguiDraw();

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
	
	// カメラ
	camera_.Update();
	
	// スカイドーム
	skydome_->Update();

	// プレイヤー
	player_->Update();

	// ねじ
	for (uint32_t i = 0; i < screws_.size(); ++i) {
		screws_[i]->Update();
	}

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

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

	// スカイドーム
	if (isDrawSkydome_) {
		skydome_->Draw(camera_);
	}
	// プレイヤー
	player_->Draw(camera_);

	// ねじ
	for (uint32_t i = 0; i < screws_.size(); ++i) {
		screws_[i]->Draw(camera_);
	}

	Model::PostDraw();
	Model::PreDrawOutLine(dxCommon_->GetCommadList());

	for (uint32_t i = 0; i < screws_.size(); ++i) {
		screws_[i]->DrawOutLine(camera_, outline_);
	}
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

void TitleScene::ImguiDraw()
{

#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::Checkbox("isDrawSkydome", &isDrawSkydome_);
	ImGui::End();
#endif // _DEBUG

}

void TitleScene::ModelCreate()
{

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));
	// プレイヤー
	playerModel_.reset(Model::Create("Resources/Model/Player/Original/", "player.obj", dxCommon_, textureHandleManager_.get()));
	// ねじ
	screwModel_.reset(Model::Create("Resources/Model/nejimi2_model/", "nejimi2.obj", dxCommon_, textureHandleManager_.get()));

}

void TitleScene::MaterialCreate()
{
}

void TitleScene::TextureLoad()
{

	buttonTextureHandle_ = TextureManager::Load("Resources/Sprite/Common/controler_UI_A.png", dxCommon_, textureHandleManager_.get());
	titleTextureHandle_ = TextureManager::Load("Resources/Sprite/Title/title.png", dxCommon_, textureHandleManager_.get());

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

void TitleScene::SpriteRegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "TilteSceneSprite";

	// タイトル
	std::string objName = "TilteSprite";
	globalVariables->AddItem(groupName, objName + "Position", titlePosition_);
	globalVariables->AddItem(groupName, objName + "Size", titleSize_);

	// ボタン
	objName = "ButtonSprite";
	globalVariables->AddItem(groupName, objName + "Position", buttonPosition_);
	globalVariables->AddItem(groupName, objName + "Size", buttonSize_);

}

void TitleScene::SpriteApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "TilteSceneSprite";

	// タイトル
	std::string objName = "TilteSprite";

	titlePosition_ = globalVariables->GetVector2Value(groupName, objName + "Position");
	titleSprite_->SetPosition(titlePosition_);

	titleSize_ = globalVariables->GetVector2Value(groupName, objName + "Size");
	titleSprite_->SetSize(titleSize_);

	// ボタン
	objName = "ButtonSprite";

	buttonPosition_ = globalVariables->GetVector2Value(groupName, objName + "Position");
	buttonSprite_->SetPosition(buttonPosition_);

	buttonSize_ = globalVariables->GetVector2Value(groupName, objName + "Size");
	buttonSprite_->SetSize(buttonSize_);

}
