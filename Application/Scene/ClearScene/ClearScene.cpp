#include "ClearScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../MissionData/MissionData.h"
#include "../../ForResult/ForResult.h"
ClearScene::~ClearScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

}

void ClearScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	titlePosition_ = { 320.0f, 240.0f };
	titleTextureHandle_ = TextureManager::Load("Resources/Sprite/Result/outgame_result.png", dxCommon_, textureHandleManager_.get());
	titleSprite_.reset(Sprite::Create(titleTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	titleSize_ = titleSprite_->GetSize();

	buttonPosition_ = { 640.0f, 560.0f };
	buttonTextureHandle_ = TextureManager::Load("Resources/Sprite/Common/controler_UI_A.png", dxCommon_, textureHandleManager_.get());
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, buttonPosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonSprite_->SetTextureSize(Vector2{ 450.0f, 450.0f });
	buttonSprite_->SetSize(Vector2{ 128.0f, 128.0f });
	buttonSprite_->SetTextureLeftTop(Vector2{ 0.0f, 0.0f });
	buttonSize_ = { 128.0f, 128.0f };
	buttonAlphaT_ = 0.0f;
	buttonAlphaTSpeed_ = 0.01f;
	buttonItIncreaseAlphaT_ = true;
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	audioManager_ = std::make_unique<ClearAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();
	audioManager_->PlayWave(kClearAudioNameIndexBGM);

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
	player_ = std::make_unique<ClearPlayer>();
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> playerModels = {
		playerModels_[kPlayerPartIndexBody].get(),
		playerModels_[kPlayerPartIndexLeftLeg].get(),
		playerModels_[kPlayerPartIndexRightLeg].get(),
		playerModels_[kPlayerPartIndexMagnet].get(),
	};
	player_->Initialize(playerModels, "Player");

	// ねじ
	for (uint32_t i = 0; i < screws_.size(); ++i) {
		screws_[i] = std::make_unique<TitleObj>();
		screws_[i]->Initialize(screwModel_.get(), "Screw" + std::to_string(i));
	}

	titlePosition_ = { 320.0f, 240.0f };
	missionTextureHandle_ = TextureManager::Load("Resources/Sprite/Result/outgame_mission_TEXT.png", dxCommon_, textureHandleManager_.get());
	missionSprite_.reset(Sprite::Create(missionTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));

	numTextureHandle_ = TextureManager::Load("Resources/Sprite/Common/number.png", dxCommon_, textureHandleManager_.get());
	leftSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	rightSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	leftTenSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	rightTenSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));

	frameTextureHandle_ = TextureManager::Load("Resources/Sprite/Result/outgame_frame.png", dxCommon_, textureHandleManager_.get());
	frameSprite_.reset(Sprite::Create(frameTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	toTitleTextureHandle_ = TextureManager::Load("Resources/Sprite/Result/outgame_titleGO_UI.png", dxCommon_, textureHandleManager_.get());
	toTitleSprite_.reset(Sprite::Create(toTitleTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	lineTextureHandle_ = TextureManager::Load("Resources/Sprite/Result/outgame_line_UI.png", dxCommon_, textureHandleManager_.get());
	lineSprite_.reset(Sprite::Create(lineTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	line2Sprite_.reset(Sprite::Create(lineTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	line3Sprite_.reset(Sprite::Create(lineTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));

	connectSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	connectTenSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	connectHandSprite_.reset(Sprite::Create(numTextureHandle_, titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	connectTextSprite_.reset(Sprite::Create(TextureManager::Load("Resources/Sprite/Result/outgame_blockNum_text.png", dxCommon_, textureHandleManager_.get()), titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	
	rankTextSprite_.reset(Sprite::Create(rankTextureHandles_[clearRank_], titlePosition_, Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	
	SpriteRegisteringGlobalVariables();
	RankColorRegisteringGlobalVariables();

	SpriteApplyGlobalVariables();
	RankColorApplyGlobalVariables();

	outline_.Initialize();
	outline_.color_ = { 0.8f,0.4f,0.1f,1.0f };
	//アウトライン
	outline_.Map();

	missionClearCount_ = 0;
	missionClearNum_ = MissionData::GetInstance()->GetClearCount();
	missionMax_ = MissionData::GetInstance()->GetMax();
	//changeNumInterval_ = 30;
	frameCount_ = 0;
	isEndCountUp_ = false;
	clearRank_ = 0;
	if (missionClearNum_<=2) {
		clearRank_ = 0;
	}
	else if(missionClearNum_ <= 8) {
		clearRank_ = 1;
	}
	else if (missionClearNum_ <= 13) {
		clearRank_ = 2;
	}
	else {
		clearRank_ = 3;
	}
#ifdef _DEBUG
	if (input_->PushKey(DIK_0)) {
		clearRank_ = 0;
	}
	if (input_->PushKey(DIK_1)) {
		clearRank_ = 1;
	}
	if (input_->PushKey(DIK_2)) {
		clearRank_ = 2;
	}
	if (input_->PushKey(DIK_3)) {
		clearRank_ = 3;
	}
#endif // _DEBUG
	rankTextSprite_->SetTextureHandle(rankTextureHandles_[clearRank_]);
	rankTextSprite_->SetIsInvisible(true);

}

void ClearScene::Update()
{

#ifdef _DEBUG
	SpriteApplyGlobalVariables();
	RankColorApplyGlobalVariables();
#endif // _DEBUG
	RankSColorAnimation();

	ImguiDraw();

	if ((input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) &&
		requestSceneNo == kClear && isStamped_) {
		// 行きたいシーンへ
		requestSceneNo = kTitle;
		audioManager_->PlayWave(kClearAudioNameIndexDecision);
	}

	// BGM音量下げる
	if (requestSceneNo == kTitle && isDecreasingVolume) {
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
	toTitleSprite_->SetColor(buttonColor_);
	//数字
	if (missionClearCount_ < missionClearNum_) {
		if (frameCount_++ > changeNumInterval_) {
			frameCount_ = 0;
			missionClearCount_++;
		}
	}
	else {
		isEndCountUp_ = true;
	}
	Vector2 size = {128.0f,128.0f};
	Vector2 leftTop = {128.0f,0.0f};
	leftTop.x = 128.0f * static_cast<float>(missionClearCount_ % 10);
	leftSprite_->SetTextureLeftTop(leftTop);
	leftSprite_->SetTextureSize(size);
	leftTop.x = 128.0f * static_cast<float>(missionClearCount_ / 10);
	leftTenSprite_->SetTextureLeftTop(leftTop);
	leftTenSprite_->SetTextureSize(size);

	leftTop.x = 128.0f * static_cast<float>(missionMax_ % 10);
	rightSprite_->SetTextureLeftTop(leftTop);
	rightSprite_->SetTextureSize(size);
	leftTop.x = 128.0f * static_cast<float>(missionMax_ / 10);
	rightTenSprite_->SetTextureLeftTop(leftTop);
	rightTenSprite_->SetTextureSize(size);

	leftTop.x = 128.0f * static_cast<float>(ForResult::GetInstance()->connectMax_ / 100);
	connectHandSprite_->SetTextureLeftTop(leftTop);
	connectHandSprite_->SetTextureSize(size);

	leftTop.x = 128.0f * static_cast<float>((ForResult::GetInstance()->connectMax_ % 100) / 10);
	connectTenSprite_->SetTextureLeftTop(leftTop);
	connectTenSprite_->SetTextureSize(size);

	leftTop.x = 128.0f * static_cast<float>((ForResult::GetInstance()->connectMax_ % 10));
	connectSprite_->SetTextureLeftTop(leftTop);
	connectSprite_->SetTextureSize(size);

	if (isEndCountUp_) {
		RankStamp();
	}
	
}

void ClearScene::Draw()
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
	frameSprite_->Draw();
	leftSprite_->Draw();
	if (leftTenSprite_->GetTextureLeftTop().x) {
		leftTenSprite_->Draw();
	}
	rightSprite_->Draw();
	if (rightTenSprite_->GetTextureLeftTop().x) {
		rightTenSprite_->Draw();
	}
	missionSprite_->Draw();
	lineSprite_->Draw();
	line2Sprite_->Draw();
	line3Sprite_->Draw();
	if (isStamped_) {
		buttonSprite_->Draw();
		toTitleSprite_->Draw();
	}
	connectSprite_->Draw();
	if (connectTenSprite_->GetTextureLeftTop().x) {
		connectTenSprite_->Draw();
	}
	if (connectHandSprite_->GetTextureLeftTop().x) {
		connectHandSprite_->Draw();
	}
	connectTextSprite_->Draw();
	rankTextSprite_->Draw();
	
	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ClearScene::ImguiDraw()
{

#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::Checkbox("isDrawSkydome", &isDrawSkydome_);
	ImGui::End();
#endif // _DEBUG

}

void ClearScene::ModelCreate()
{

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));
	// プレイヤー
	playerModels_[kPlayerPartIndexBody].reset(Model::Create("Resources/Model/Player/Body", "playerBody.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexLeftLeg].reset(Model::Create("Resources/Model/Player/LeftLeg/", "playerLeftLeg.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexRightLeg].reset(Model::Create("Resources/Model/Player/RightLeg/", "playerRightLeg.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexMagnet].reset(Model::Create("Resources/Model/Player/Magnet/", "playerMagnet.obj", dxCommon_, textureHandleManager_.get()));

	// ねじ
	screwModel_.reset(Model::Create("Resources/Model/nejimi2_model/", "nejimi2.obj", dxCommon_, textureHandleManager_.get()));

}

void ClearScene::MaterialCreate()
{
}

void ClearScene::TextureLoad()
{

	rankTextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Result/result_c.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Result/result_b.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Result/result_a.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Result/result_s.png", dxCommon_, textureHandleManager_.get()),
	};

}

void ClearScene::LowerVolumeBGM()
{

	const uint32_t startHandleIndex = 13;

	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kClearAudioNameIndexBGM + startHandleIndex) {
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

void ClearScene::SpriteRegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "ClearSceneSprite";

	// タイトル
	std::string objName = "TilteSprite";
	globalVariables->AddItem(groupName, objName + "Position", titlePosition_);
	globalVariables->AddItem(groupName, objName + "Size", titleSize_);

	// ボタン
	objName = "ButtonSprite";
	globalVariables->AddItem(groupName, objName + "Position", buttonPosition_);
	globalVariables->AddItem(groupName, objName + "Size", buttonSize_);

	const std::string groupName2 = "ClearSceneSprite";

	//ミッション
	objName = "MissionSprite";
	globalVariables->AddItem(groupName2, objName + "Position", missionPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", missionSize_);

	objName = "LeftSprite";
	globalVariables->AddItem(groupName2, objName + "Position", leftPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", leftSize_);

	objName = "RightSprite";
	globalVariables->AddItem(groupName2, objName + "Position", rightPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", rightSize_);

	objName = "LeftTenSprite";
	globalVariables->AddItem(groupName2, objName + "Position", leftTenPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", leftTenSize_);

	objName = "RightTenSprite";
	globalVariables->AddItem(groupName2, objName + "Position", rightTenPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", rightTenSize_);

	objName = "ToTitleSprite";
	globalVariables->AddItem(groupName2, objName + "Position", toTitlePosition_);
	globalVariables->AddItem(groupName2, objName + "Size", toTitleSize_);

	objName = "Line1Sprite";
	globalVariables->AddItem(groupName2, objName + "Position", linePosition_);
	globalVariables->AddItem(groupName2, objName + "Size", lineSize_);
	objName = "Line2Sprite";
	globalVariables->AddItem(groupName2, objName + "Position", line2Position_);
	globalVariables->AddItem(groupName2, objName + "Size", line2Size_);
	objName = "Line3Sprite";
	globalVariables->AddItem(groupName2, objName + "Position", line2Position_);
	globalVariables->AddItem(groupName2, objName + "Size", line2Size_);

	objName = "ConnectSprite";
	globalVariables->AddItem(groupName2, objName + "Position", connectPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", connectSize_);
	objName = "ConnectTenSprite";
	globalVariables->AddItem(groupName2, objName + "Position", connectTenPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", connectTenSize_);
	objName = "ConnectHandSprite";
	globalVariables->AddItem(groupName2, objName + "Position", connectHandPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", connectHandSize_);

	objName = "ConnectTextSprite";
	globalVariables->AddItem(groupName2, objName + "Position", connectTextPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", connectTextSize_);

	objName = "RankTextSprite";
	globalVariables->AddItem(groupName2, objName + "Position", rankTextPosition_);
	globalVariables->AddItem(groupName2, objName + "Size", rankTextSize_);


}

void ClearScene::SpriteApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "ClearSceneSprite";

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

	const std::string groupName2 = "ClearSceneSprite";

	//ミッション
	objName = "MissionSprite";
	
	missionPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	missionSprite_->SetPosition(missionPosition_);

	missionSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	missionSprite_->SetSize(missionSize_);

	objName = "LeftSprite";
	leftPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	leftSprite_->SetPosition(leftPosition_);
	leftSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	leftSprite_->SetSize(leftSize_);

	objName = "RightSprite";
	rightPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	rightSprite_->SetPosition(rightPosition_);
	rightSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	rightSprite_->SetSize(rightSize_);

	objName = "LeftTenSprite";
	leftTenPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	leftTenSprite_->SetPosition(leftTenPosition_);
	leftTenSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	leftTenSprite_->SetSize(leftTenSize_);

	objName = "RightTenSprite";
	rightTenPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	rightTenSprite_->SetPosition(rightTenPosition_);
	rightTenSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	rightTenSprite_->SetSize(rightTenSize_);

	objName = "ToTitleSprite";
	toTitlePosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	toTitleSprite_->SetPosition(toTitlePosition_);
	toTitleSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	toTitleSprite_->SetSize(toTitleSize_);

	objName = "Line1Sprite";
	linePosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	lineSprite_->SetPosition(linePosition_);
	lineSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	lineSprite_->SetSize(lineSize_);

	objName = "Line2Sprite";
	line2Position_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	line2Sprite_->SetPosition(line2Position_);
	line2Size_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	line2Sprite_->SetSize(line2Size_);

	objName = "Line3Sprite";
	line3Position_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	line3Sprite_->SetPosition(line3Position_);
	line3Size_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	line3Sprite_->SetSize(line3Size_);

	const std::string groupName3 = "UI";
	objName = "UIIndexFrame";
	framePosition_ = globalVariables->GetVector2Value(groupName3, objName + "position");
	frameSprite_->SetPosition(framePosition_);
	frameSize_ = globalVariables->GetVector2Value(groupName3, objName + "size");
	frameSprite_->SetSize(frameSize_);

	objName = "ConnectSprite";
	connectPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	connectSprite_->SetPosition(connectPosition_);
	connectSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	connectSprite_->SetSize(connectSize_);
	objName = "ConnectTenSprite";
	connectTenPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	connectTenSprite_->SetPosition(connectTenPosition_);
	connectTenSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	connectTenSprite_->SetSize(connectTenSize_);
	objName = "ConnectHandSprite";
	connectHandPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	connectHandSprite_->SetPosition(connectHandPosition_);
	connectHandSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	connectHandSprite_->SetSize(connectHandSize_);

	objName = "ConnectTextSprite";
	connectTextPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	connectTextSprite_->SetPosition(connectTextPosition_);
	connectTextSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	connectTextSprite_->SetSize(connectTextSize_);

	objName = "RankTextSprite";
	rankTextPosition_ = globalVariables->GetVector2Value(groupName2, objName + "Position");
	rankTextSprite_->SetPosition(rankTextPosition_);
	rankTextSize_ = globalVariables->GetVector2Value(groupName2, objName + "Size");
	rankTextSprite_->SetSize(rankTextSize_);

	const std::string groupName4 = "ClearScene";
	globalVariables->AddItem(groupName4, "ChangeNumInterval", changeNumInterval_);
	changeNumInterval_ = globalVariables->GetIntValue(groupName4, "ChangeNumInterval");
}

void ClearScene::RankStamp()
{

	float speed = 0.05f;

	stampT_ += speed;
	if (stampT_ >= 1.0f) {
		stampT_ = 1.0f;
		if (stampCooltime_ >= 1.0f) {
			stampCooltime_ = 0.0f;
			isStamped_ = true;
		}
		else {
			stampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 800.0f, 600.0f };
	Vector2 end = { 480.0f, 360.0f };
	rankTextSprite_->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, stampT_));
	rankTextSprite_->SetIsInvisible(false);

}

void ClearScene::RankColorRegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "RankColor";

	std::string objName = "C";
	globalVariables->AddItem(groupName, objName, rankColors_[0]);
	objName = "B";
	globalVariables->AddItem(groupName, objName, rankColors_[1]);
	objName = "A";
	globalVariables->AddItem(groupName, objName, rankColors_[2]);
	objName = "S";
	globalVariables->AddItem(groupName, objName, rankColors_[3]);

}

void ClearScene::RankColorApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const std::string groupName = "RankColor";

	std::string objName = "C";
	rankColors_[0] = globalVariables->GetVector3Value(groupName, objName);
	objName = "B";
	rankColors_[1] = globalVariables->GetVector3Value(groupName, objName);
	objName = "A";
	rankColors_[2] = globalVariables->GetVector3Value(groupName, objName);
	objName = "S";
	rankColors_[3] = globalVariables->GetVector3Value(groupName, objName);

	Vector4 color = { rankColors_[clearRank_].x,rankColors_[clearRank_].y,rankColors_[clearRank_].z, 1.0f };
	rankTextSprite_->SetColor(color);

}

void ClearScene::RankSColorAnimation() {
	static int colorPhase_ = 0;
	static float color_ = 0.0f;
	switch (colorPhase_)
	{
	case 0:
		rankColors_[3] = Vector3{1.0f - color_,color_, 0.0f};
		break;
	case 1:
		rankColors_[3] = Vector3{0.0f,1.0f-color_, color_ };
		break;
	case 2:
		rankColors_[3] = Vector3{color_,0.0f, 1.0f-color_};
		break;
	default:
		break;
	}
	rankColors_[3] = Vector3Calc::Multiply(0.5f, rankColors_[3]);
	rankColors_[3] = Vector3Calc::Add(rankColors_[3],Vector3{0.5f,0.5f,0.5f});
	color_ += 0.02f;
	if (color_ > 1.0f) {
		color_ = 0;
		colorPhase_++;
		if (colorPhase_ > 2) {
			colorPhase_ = 0;
		}
	}
	Vector4 color = { rankColors_[clearRank_].x,rankColors_[clearRank_].y,rankColors_[clearRank_].z, 1.0f };
	rankTextSprite_->SetColor(color);
}