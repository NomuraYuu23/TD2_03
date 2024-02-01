#include "UIManager.h"
#include "UIScrew/UIScrew.h"
#include "UINumber/UINumber.h"
#include "UISymbol/UISymbol.h"
#include "UIFrame/UIFrame.h"
#include "UIEnergyPlate/UIEnergyPlate.h"
#include "UIEnergy/UIEnergy.h"
#include "UIMissionFrame/UIMissionFrame.h"
#include "UIMissionText/UIMissionText.h"
#include "UIMissionClear/UIMissionClear.h"
#include "../../Engine/Math/Ease.h"

void UIManager::Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		UIInitPositions_[i] = UIs_[i]->GetPosition();
	}

	blockMissionBeenUpdate_ = false;

	blockMissionBeenUpdateColor_ = {1.0f,1.0f,1.0f,1.0f };

	blockMissionBeenUpdateFadeIn_ = false;

	IsBlockStamped_ = false;

	blockStampT_ = 0.0f;

	blockStampCooltime_ = 0.0f;

	// ミッション更新中
	pointMissionBeenUpdate_ = false;
	// ミッション更新中の色
	pointMissionBeenUpdateColor_ = { 1.0f,1.0f,1.0f,1.0f };
	// ミッション
	pointMissionBeenUpdateFadeIn_ = false;
	// ハンコが押された
	IsPointStamped_ = false;
	// ハンコの媒介変数
	pointStampT_ = 0.0f;
	// ハンコのクールタイム
	pointStampCooltime_ = 0.0f;

}

void UIManager::Update(const UIManagerUpdateDesc& uiManagerUpdateDesc)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// フレーム
	UIs_[kUIIndexFrame]->Update();
	// 残りねじアイコン
	UIs_[kUIIndexRemainingScrewsIcon]->Update();
	// 残りねじのx
	UIs_[kUIIndexRemainingScrewsCross]->Update();
	// 残りねじの数字10の位
	leftTop.x = 128.0f * static_cast<float>(uiManagerUpdateDesc.screwCount / 10);
	UIs_[kUIIndexRemainingScrewsTensPlace]->Update(leftTop);
	// 残りねじの数字1の位
	leftTop.x = 128.0f * static_cast<float>(uiManagerUpdateDesc.screwCount % 10);
	UIs_[kUIIndexRemainingScrewsOnesPlace]->Update(leftTop);
	// タイマー分
	leftTop.x = 128.0f * static_cast<float>(uiManagerUpdateDesc.timer / 60);
	UIs_[kUIIndexTimerMinutes]->Update(leftTop);
	// タイマー秒10の位
	leftTop.x = 128.0f * static_cast<float>((uiManagerUpdateDesc.timer % 60) / 10);
	UIs_[kUIIndexTimerSecondsTensPlace]->Update(leftTop);
	// タイマー秒1の位
	leftTop.x = 128.0f * static_cast<float>((uiManagerUpdateDesc.timer % 60) % 10);
	UIs_[kUIIndexTimerSecondsOnesPlace]->Update(leftTop);
	// タイマーコロン
	UIs_[kUIIndexTimerColon]->Update();

	// ミッションクリア
	UIs_[kUIIndexMissionBlockClear]->Update();

	// ブロックアップデート確認
	if (uiManagerUpdateDesc.missionBlockBeenUpdated) {
		blockMissionBeenUpdate_ = true;
		UIs_[kUIIndexMissionBlockClear]->SetIsInvisible(false);
		blockStampT_ = 0.0f;
		audioManager_->PlayWave(kGameAudioNameIndexMissionClear);
	}

	//ブロックアップデート中
	if (blockMissionBeenUpdate_) {
		BlockMissionUpdate(uiManagerUpdateDesc.missionBlockCount, uiManagerUpdateDesc.blockCount, uiManagerUpdateDesc.isCompleteBlock);
	}
	else {
		// ミッションフレーム
		UIs_[kUIIndexMissionBlockFrame]->Update();
		// ミッションテキスト
		UIs_[kUIIndexMissionBlockText]->Update();

		// ミッションブロック更新
		MissionBlockCountUpdate(uiManagerUpdateDesc.missionBlockCount);

		// 持っているブロックの数更新
		BlockCountUpdate(uiManagerUpdateDesc.blockCount);

	}

	// ポイントアップデート確認
	if (uiManagerUpdateDesc.missionPointBeenUpdated) {
		pointMissionBeenUpdate_ = true;
		UIs_[kUIIndexMissionPointClear]->SetIsInvisible(false);
		pointStampT_ = 0.0f;
		audioManager_->PlayWave(kGameAudioNameIndexMissionClear);
		audioManager_->PlayWave(kGameAudioNameIndexFlagGet);
	}

	//ぽいんとアップデート中
	if (pointMissionBeenUpdate_) {
		PointMissionUpdate(uiManagerUpdateDesc.isCompletePoint, uiManagerUpdateDesc.missionNumPoint_);
	}
	else {
		// ミッション2フレーム
		UIs_[kUIIndexMissionPointFrame]->Update();
		// ミッションテキスト
		UIs_[kUIIndexMissionPointText]->Update();

		MissionPointNumUpdate(uiManagerUpdateDesc.missionNumPoint_);

	}

	if (uiManagerUpdateDesc.isCompleteBlock) {
		UIs_[kUIIndexMissionBlockClear]->SetTextureHandle(textureHandles_[kUITextureHandleIndexMissionComplete]);
		UIs_[kUIIndexMissionBlockClear]->SetSize(Vector2{ 144.0f, 84.0f });
		UIs_[kUIIndexMissionBlockClear]->SetRotate(0.0f);
	}

	if (uiManagerUpdateDesc.isCompletePoint) {
		UIs_[kUIIndexMissionPointClear]->SetTextureHandle(textureHandles_[kUITextureHandleIndexMissionComplete]);
		UIs_[kUIIndexMissionPointClear]->SetSize(Vector2{ 144.0f, 84.0f });
		UIs_[kUIIndexMissionPointClear]->SetRotate(0.0f);
	}

}

void UIManager::Draw()
{

	for (uint32_t i = 0;i < UIIndex::kUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}

}

void UIManager::UIInitialize()
{

	Vector2 leftTop = { 0.0f, 0.0f };
	Vector2 numberSize = { 128.0f, 128.0f };
	Vector2 symbolSize = { 192.0f, 192.0f };
	Vector2 frameSize = { 1280.0f, 720.0f };
	Vector2 missionFrameSize = { 1748.0f, 300.0f };
	Vector2 missionTextSize = { 1748.0f, 300.0f};
	Vector2 missionClearSize = {800.0f, 700.0f};
	Vector2 missionFlagColorSize = {135.0f, 135.0f};

	// フレーム
	leftTop = { 0.0f, 0.0f };
	UIs_[kUIIndexFrame] = std::make_unique<UIFrame>();
	UIs_[kUIIndexFrame]->Initialize(textureHandles_[kUITextureHandleIndexFrame], "UIIndexFrame", frameSize, leftTop);

	// 残りねじアイコン
	UIs_[kUIIndexRemainingScrewsIcon] = std::make_unique<UIScrew>();
	UIs_[kUIIndexRemainingScrewsIcon]->Initialize(textureHandles_[kUITextureHandleIndexScrews], "UIScrewsIcon");

	// 残りねじのx
	leftTop = { 192.0f, 0.0f };
	UIs_[kUIIndexRemainingScrewsCross] = std::make_unique<UISymbol>();
	UIs_[kUIIndexRemainingScrewsCross]->Initialize(textureHandles_[kUITextureHandleIndexSymbol], "UIScrewsCross", symbolSize, leftTop);

	// 残りねじの数字10の位
	UIs_[kUIIndexRemainingScrewsTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexRemainingScrewsTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIScrewsTensPlace", numberSize, leftTop);

	// 残りねじの数字1の位
	UIs_[kUIIndexRemainingScrewsOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexRemainingScrewsOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIScrewsOnesPlace", numberSize, leftTop);

	// タイマー分
	UIs_[kUIIndexTimerMinutes] = std::make_unique<UINumber>();
	UIs_[kUIIndexTimerMinutes]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UITimerMinutes", numberSize, leftTop);

	// タイマー秒10の位
	UIs_[kUIIndexTimerSecondsTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexTimerSecondsTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UITimerSecondsTensPlace", numberSize, leftTop);

	// タイマー秒1の位
	UIs_[kUIIndexTimerSecondsOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexTimerSecondsOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UITimerSecondsOnesPlace", numberSize, leftTop);

	// タイマーコロン
	leftTop = { 0.0f, 0.0f };
	UIs_[kUIIndexTimerColon] = std::make_unique<UISymbol>();
	UIs_[kUIIndexTimerColon]->Initialize(textureHandles_[kUITextureHandleIndexSymbol], "UITimerColon", symbolSize, leftTop);

	// ブロックミッションフレーム
	UIs_[kUIIndexMissionBlockFrame] = std::make_unique<UIMissionFrame>();
	UIs_[kUIIndexMissionBlockFrame]->Initialize(textureHandles_[kUITextureHandleIndexMissionFrame], "UIMissionFrame", missionFrameSize, leftTop);

	// ミッションテキスト
	UIs_[kUIIndexMissionBlockText] = std::make_unique<UIMissionText>();
	UIs_[kUIIndexMissionBlockText]->Initialize(textureHandles_[kUITextureHandleIndexBlockMissionText], "UIMissionText", missionTextSize, leftTop);

	// ミッション番号1の位
	UIs_[kUIIndexMissionNumTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionNumTensPlace", numberSize, leftTop);

	// ミッション番号10の位
	UIs_[kUIIndexMissionNumOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionNumOnesPlace", numberSize, leftTop);

	// ミッション分母10の位
	UIs_[kUIIndexMissionDenominatorTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionDenominatorTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionDenominatorTensPlace", numberSize, leftTop);
	// ミッション分母1の位
	UIs_[kUIIndexMissionDenominatorOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionDenominatorOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionDenominatorOnesPlace", numberSize, leftTop);
	// ミッション分子10の位
	UIs_[kUIIndexMissionNumeratorTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumeratorTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionNumeratorTensPlace", numberSize, leftTop);
	// ミッション分子1の位
	UIs_[kUIIndexMissionNumeratorOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumeratorOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexMissionNum], "UIMissionNumeratorOnesPlace", numberSize, leftTop);

	// ブロックミッションクリア
	UIs_[kUIIndexMissionBlockClear] = std::make_unique<UIMissionClear>();
	UIs_[kUIIndexMissionBlockClear]->Initialize(textureHandles_[kUITextureHandleIndexMissionClear], "UIMissionClear", missionClearSize, leftTop);
	UIs_[kUIIndexMissionBlockClear]->SetIsInvisible(true);

	// ポイントミッションフレーム
	UIs_[kUIIndexMissionPointFrame] = std::make_unique<UIMissionFrame>();
	UIs_[kUIIndexMissionPointFrame]->Initialize(textureHandles_[kUITextureHandleIndexMissionFrame], "UIMission2Frame", missionFrameSize, leftTop);

	// ポイントミッションテキスト
	UIs_[kUIIndexMissionPointText] = std::make_unique<UIMissionText>();
	UIs_[kUIIndexMissionPointText]->Initialize(textureHandles_[kUITextureHandleIndexPointMissionText], "UIMission2Text", missionTextSize, leftTop);

	// ミッション旗色
	UIs_[kUIindexMissionFlagColor] = std::make_unique<UIMissionText>();
	UIs_[kUIindexMissionFlagColor]->Initialize(textureHandles_[kUITextureHandleIndexFlagColor], "UIMissionFlagColor", missionFlagColorSize, leftTop);
	
	// ポイントミッションクリア
	UIs_[kUIIndexMissionPointClear] = std::make_unique<UIMissionClear>();
	UIs_[kUIIndexMissionPointClear]->Initialize(textureHandles_[kUITextureHandleIndexMissionClear], "UIMission2Clear", missionClearSize, leftTop);
	UIs_[kUIIndexMissionPointClear]->SetIsInvisible(true);

}

void UIManager::BlockMissionUpdate(uint32_t missionBlockCount, uint32_t blockCount, bool isCompleteBlock)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// 持っているブロックの数更新
	BlockCountUpdate(blockCount);

	// クリア
	if (!blockMissionBeenUpdateFadeIn_) {

		if (!IsBlockStamped_) {
			// ハンコ
			BlockStamp();
		}
		else {
			if (isCompleteBlock) {
				blockMissionBeenUpdate_ = false;
				return;
			}
			blockMissionBeenUpdateColor_.w -= 0.05f;
			if (blockMissionBeenUpdateColor_.w <= 0.0f) {
				blockMissionBeenUpdateColor_.w = 0.0f;
				blockMissionBeenUpdateFadeIn_ = true;
				UIs_[kUIIndexMissionBlockClear]->SetIsInvisible(true);
				// ミッションブロック更新
				MissionBlockCountUpdate(missionBlockCount);
				audioManager_->PlayWave(kGameAudioNameIndexMissionOccurrrence);
			}

			BlockClearMissionUpdate((1.0f - blockMissionBeenUpdateColor_.w));
		}

	}
	// 新しいものが飛んでくる
	else {
		blockMissionBeenUpdateColor_.w += 0.05f;
		if (blockMissionBeenUpdateColor_.w >= 1.0f) {
			blockMissionBeenUpdateColor_.w = 1.0f;
			blockMissionBeenUpdateFadeIn_ = false;
			blockMissionBeenUpdate_ = false;
			IsBlockStamped_ = false;

			// ミッションクリア
			UIs_[kUIIndexMissionBlockClear]->SetPosition(UIInitPositions_[kUIIndexMissionBlockClear]);

		}

		BlockNewMissionMove(blockMissionBeenUpdateColor_.w);

	}

	// ミッションフレーム
	UIs_[kUIIndexMissionBlockFrame]->SetColor(blockMissionBeenUpdateColor_);
	// ミッションテキスト
	UIs_[kUIIndexMissionBlockText]->SetColor(blockMissionBeenUpdateColor_);

	// ミッション番号10の位
	UIs_[kUIIndexMissionNumTensPlace]->SetColor(blockMissionBeenUpdateColor_);
	// ミッション番号1の位
	UIs_[kUIIndexMissionNumOnesPlace]->SetColor(blockMissionBeenUpdateColor_);
	// ミッション分母10の位
	UIs_[kUIIndexMissionDenominatorTensPlace]->SetColor(blockMissionBeenUpdateColor_);
	// ミッション分母1の位
	UIs_[kUIIndexMissionDenominatorOnesPlace]->SetColor(blockMissionBeenUpdateColor_);
	// ミッション分子10の位
	UIs_[kUIIndexMissionNumeratorTensPlace]->SetColor(blockMissionBeenUpdateColor_);
	// ミッション分子1の位
	UIs_[kUIIndexMissionNumeratorOnesPlace]->SetColor(blockMissionBeenUpdateColor_);

}

void UIManager::BlockCountUpdate(uint32_t blockCount)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// ミッション分子10の位
	leftTop.x = 128.0f * static_cast<float>(blockCount / 10);
	UIs_[kUIIndexMissionNumeratorTensPlace]->Update(leftTop);
	if (leftTop.x == 0.0f) {
		UIs_[kUIIndexMissionNumeratorTensPlace]->SetIsInvisible(true);
	}
	else {
		UIs_[kUIIndexMissionNumeratorTensPlace]->SetIsInvisible(false);
	}
	// ミッション分子1の位
	leftTop.x = 128.0f * static_cast<float>(blockCount % 10);
	UIs_[kUIIndexMissionNumeratorOnesPlace]->Update(leftTop);
	if (UIs_[kUIIndexMissionNumeratorTensPlace]->GetIsInvisible()) {
		Vector2 pos = {
			(UIs_[kUIIndexMissionNumeratorTensPlace]->GetPosition().x + UIs_[kUIIndexMissionNumeratorOnesPlace]->GetPosition().x) / 2.0f,
			(UIs_[kUIIndexMissionNumeratorTensPlace]->GetPosition().y + UIs_[kUIIndexMissionNumeratorOnesPlace]->GetPosition().y) / 2.0f };
		UIs_[kUIIndexMissionNumeratorOnesPlace]->SetPosition(pos);
	}

}

void UIManager::MissionBlockCountUpdate(uint32_t missionBlockCount)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// ミッション番号10の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount / 10);
	UIs_[kUIIndexMissionNumTensPlace]->Update(leftTop);
	if (leftTop.x == 0.0f) {
		UIs_[kUIIndexMissionNumTensPlace]->SetIsInvisible(true);
	}
	else {
		UIs_[kUIIndexMissionNumTensPlace]->SetIsInvisible(false);
	}
	// ミッション番号1の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount % 10);
	UIs_[kUIIndexMissionNumOnesPlace]->Update(leftTop);
	if (UIs_[kUIIndexMissionNumTensPlace]->GetIsInvisible()) {
		Vector2 pos = {
			(UIs_[kUIIndexMissionNumTensPlace]->GetPosition().x + UIs_[kUIIndexMissionNumOnesPlace]->GetPosition().x) / 2.0f,
			(UIs_[kUIIndexMissionNumTensPlace]->GetPosition().y + UIs_[kUIIndexMissionNumOnesPlace]->GetPosition().y) / 2.0f };
		UIs_[kUIIndexMissionNumOnesPlace]->SetPosition(pos);
	}

	// ミッション分母10の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount / 10);
	UIs_[kUIIndexMissionDenominatorTensPlace]->Update(leftTop);
	if (leftTop.x == 0.0f) {
		UIs_[kUIIndexMissionDenominatorTensPlace]->SetIsInvisible(true);
	}
	else {
		UIs_[kUIIndexMissionDenominatorTensPlace]->SetIsInvisible(false);
	}
	// ミッション分母1の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount % 10);
	UIs_[kUIIndexMissionDenominatorOnesPlace]->Update(leftTop);
	if (UIs_[kUIIndexMissionDenominatorTensPlace]->GetIsInvisible()) {
		Vector2 pos = {
			(UIs_[kUIIndexMissionDenominatorTensPlace]->GetPosition().x + UIs_[kUIIndexMissionDenominatorOnesPlace]->GetPosition().x) / 2.0f,
			(UIs_[kUIIndexMissionDenominatorTensPlace]->GetPosition().y + UIs_[kUIIndexMissionDenominatorOnesPlace]->GetPosition().y) / 2.0f };
		UIs_[kUIIndexMissionDenominatorOnesPlace]->SetPosition(pos);
	}

}

void UIManager::BlockNewMissionMove(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addStart = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionBlockFrame];
	end = UIInitPositions_[kUIIndexMissionBlockFrame];
	start.x += addStart;
	UIs_[kUIIndexMissionBlockFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionBlockText];
	end = UIInitPositions_[kUIIndexMissionBlockText];
	start.x += addStart;
	UIs_[kUIIndexMissionBlockText]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション番号10の位
	start = UIInitPositions_[kUIIndexMissionNumTensPlace];
	end = UIInitPositions_[kUIIndexMissionNumTensPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionNumTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション番号1の位
	start = UIInitPositions_[kUIIndexMissionNumOnesPlace];
	end = UIInitPositions_[kUIIndexMissionNumOnesPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionNumOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション分母10の位
	start = UIInitPositions_[kUIIndexMissionDenominatorTensPlace];
	end = UIInitPositions_[kUIIndexMissionDenominatorTensPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionDenominatorTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション分母1の位
	start = UIInitPositions_[kUIIndexMissionDenominatorOnesPlace];
	end = UIInitPositions_[kUIIndexMissionDenominatorOnesPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionDenominatorOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション分子10の位
	start = UIInitPositions_[kUIIndexMissionNumeratorTensPlace];
	end = UIInitPositions_[kUIIndexMissionNumeratorTensPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionNumeratorTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッション分子1の位
	start = UIInitPositions_[kUIIndexMissionNumeratorOnesPlace];
	end = UIInitPositions_[kUIIndexMissionNumeratorOnesPlace];
	start.x += addStart;
	UIs_[kUIIndexMissionNumeratorOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));

}

void UIManager::BlockClearMissionUpdate(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addEnd = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionBlockFrame];
	end = UIInitPositions_[kUIIndexMissionBlockFrame];
	end.x += addEnd;
	UIs_[kUIIndexMissionBlockFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionBlockText];
	end = UIInitPositions_[kUIIndexMissionBlockText];
	end.x += addEnd;
	UIs_[kUIIndexMissionBlockText]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション番号10の位
	start = UIInitPositions_[kUIIndexMissionNumTensPlace];
	end = UIInitPositions_[kUIIndexMissionNumTensPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionNumTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション番号1の位
	start = UIInitPositions_[kUIIndexMissionNumOnesPlace];
	end = UIInitPositions_[kUIIndexMissionNumOnesPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionNumOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション分母10の位
	start = UIInitPositions_[kUIIndexMissionDenominatorTensPlace];
	end = UIInitPositions_[kUIIndexMissionDenominatorTensPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionDenominatorTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション分母1の位
	start = UIInitPositions_[kUIIndexMissionDenominatorOnesPlace];
	end = UIInitPositions_[kUIIndexMissionDenominatorOnesPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionDenominatorOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション分子10の位
	start = UIInitPositions_[kUIIndexMissionNumeratorTensPlace];
	end = UIInitPositions_[kUIIndexMissionNumeratorTensPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionNumeratorTensPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッション分子1の位
	start = UIInitPositions_[kUIIndexMissionNumeratorOnesPlace];
	end = UIInitPositions_[kUIIndexMissionNumeratorOnesPlace];
	end.x += addEnd;
	UIs_[kUIIndexMissionNumeratorOnesPlace]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	// ミッションクリア
	start = UIInitPositions_[kUIIndexMissionBlockClear];
	end = UIInitPositions_[kUIIndexMissionBlockClear];
	end.x += addEnd;
	UIs_[kUIIndexMissionBlockClear]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

}

void UIManager::BlockStamp()
{

	float speed = 0.05f;

	blockStampT_ += speed;
	if (blockStampT_ >= 1.0f) {
		blockStampT_ = 1.0f;
		if (blockStampCooltime_ >= 1.0f) {
			blockStampCooltime_ = 0.0f;
			IsBlockStamped_ = true;
		}
		else {
			blockStampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 120.0f, 105.0f };
	Vector2 end = { 96.0f, 84.0f };
	UIs_[kUIIndexMissionBlockClear]->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, blockStampT_));

}

void UIManager::PointMissionUpdate(bool isCompletePoint, size_t num)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// クリア
	if (!pointMissionBeenUpdateFadeIn_) {

		if (!IsPointStamped_) {
			// ハンコ
			PointStamp();
		}
		else {
			if (isCompletePoint) {
				pointMissionBeenUpdate_ = false;
				return;
			}
			pointMissionBeenUpdateColor_.w -= 0.05f;
			if (pointMissionBeenUpdateColor_.w <= 0.0f) {
				pointMissionBeenUpdateColor_.w = 0.0f;
				pointMissionBeenUpdateFadeIn_ = true;
				UIs_[kUIIndexMissionPointClear]->SetIsInvisible(true);

				MissionPointNumUpdate(num);
				audioManager_->PlayWave(kGameAudioNameIndexMissionOccurrrence);
			}

			PointClearMissionUpdate((1.0f - pointMissionBeenUpdateColor_.w));
		}

	}
	// 新しいものが飛んでくる
	else {
		pointMissionBeenUpdateColor_.w += 0.05f;
		if (pointMissionBeenUpdateColor_.w >= 1.0f) {
			pointMissionBeenUpdateColor_.w = 1.0f;
			pointMissionBeenUpdateFadeIn_ = false;
			pointMissionBeenUpdate_ = false;
			IsPointStamped_ = false;

			// ミッションクリア
			UIs_[kUIIndexMissionPointClear]->SetPosition(UIInitPositions_[kUIIndexMissionPointClear]);

		}

		PointNewMissionMove(blockMissionBeenUpdateColor_.w);

	}

	// ミッションフレーム
	UIs_[kUIIndexMissionPointText]->SetColor(pointMissionBeenUpdateColor_);
	// ミッションテキスト
	UIs_[kUIIndexMissionPointText]->SetColor(pointMissionBeenUpdateColor_);
	// ミッション旗色
	UIs_[kUIindexMissionFlagColor]->SetColor(pointMissionBeenUpdateColor_);


}

void UIManager::PointStamp()
{

	float speed = 0.05f;

	pointStampT_ += speed;
	if (pointStampT_ >= 1.0f) {
		pointStampT_ = 1.0f;
		if (pointStampCooltime_ >= 1.0f) {
			pointStampCooltime_ = 0.0f;
			IsPointStamped_ = true;
		}
		else {
			pointStampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 120.0f, 105.0f };
	Vector2 end = { 96.0f, 84.0f };
	UIs_[kUIIndexMissionPointClear]->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, pointStampT_));

}

void UIManager::PointClearMissionUpdate(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addEnd = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionPointFrame];
	end = UIInitPositions_[kUIIndexMissionPointFrame];
	end.x += addEnd;
	UIs_[kUIIndexMissionPointFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionPointText];
	end = UIInitPositions_[kUIIndexMissionPointText];
	end.x += addEnd;
	UIs_[kUIIndexMissionPointText]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// 旗色
	start = UIInitPositions_[kUIindexMissionFlagColor];
	end = UIInitPositions_[kUIindexMissionFlagColor];
	end.x += addEnd;
	UIs_[kUIindexMissionFlagColor]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// クリア
	start = UIInitPositions_[kUIIndexMissionPointClear];
	end = UIInitPositions_[kUIIndexMissionPointClear];
	end.x += addEnd;
	UIs_[kUIIndexMissionPointClear]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

}

void UIManager::PointNewMissionMove(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addStart = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionPointFrame];
	end = UIInitPositions_[kUIIndexMissionPointFrame];
	start.x += addStart;
	UIs_[kUIIndexMissionPointFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionPointText];
	end = UIInitPositions_[kUIIndexMissionPointText];
	start.x += addStart;
	UIs_[kUIIndexMissionPointText]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// 旗色
	start = UIInitPositions_[kUIindexMissionFlagColor];
	end = UIInitPositions_[kUIindexMissionFlagColor];
	start.x += addStart;
	UIs_[kUIindexMissionFlagColor]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));

}

void UIManager::MissionPointNumUpdate(size_t num)
{

	Vector2 leftTop = { 0.0f,0.0f };

	// ミッション番号1の位
	leftTop.x = 135.0f * num;
	UIs_[kUIindexMissionFlagColor]->Update(leftTop);

}
