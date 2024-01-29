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

	missionBeenUpdate_ = false;

	missionBeenUpdateColor_ = {1.0f,1.0f,1.0f,1.0f };

	missionBeenUpdateFadeIn_ = false;

	IsStamped_ = false;

	stampT_ = 0.0f;

	stampCooltime_ = 0.0f;

}

void UIManager::Update(uint32_t screwCount, uint32_t missionBlockCount, uint32_t blockCount, bool missionBeenUpdated, int timer)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// フレーム
	UIs_[kUIIndexFrame]->Update();
	// 残りねじアイコン
	UIs_[kUIIndexRemainingScrewsIcon]->Update();
	// 残りねじのx
	UIs_[kUIIndexRemainingScrewsCross]->Update();
	// 残りねじの数字10の位
	leftTop.x = 128.0f * static_cast<float>(screwCount / 10);
	UIs_[kUIIndexRemainingScrewsTensPlace]->Update(leftTop);
	// 残りねじの数字1の位
	leftTop.x = 128.0f * static_cast<float>(screwCount % 10);
	UIs_[kUIIndexRemainingScrewsOnesPlace]->Update(leftTop);
	// タイマー分
	leftTop.x = 128.0f * static_cast<float>(timer / 60);
	UIs_[kUIIndexTimerMinutes]->Update(leftTop);
	// タイマー秒10の位
	leftTop.x = 128.0f * static_cast<float>((timer % 60) / 10);
	UIs_[kUIIndexTimerSecondsTensPlace]->Update(leftTop);
	// タイマー秒1の位
	leftTop.x = 128.0f * static_cast<float>((timer % 60) % 10);
	UIs_[kUIIndexTimerSecondsOnesPlace]->Update(leftTop);
	// タイマーコロン
	UIs_[kUIIndexTimerColon]->Update();

	// ミッションクリア
	UIs_[kUIIndexMissionClear]->Update();

	if (missionBeenUpdated) {
		missionBeenUpdate_ = true;
		UIs_[kUIIndexMissionClear]->SetIsInvisible(false);
		stampT_ = 0.0f;
		audioManager_->PlayWave(kGameAudioNameIndexMissionClear);
	}

	//アップデート中
	if (missionBeenUpdate_) {
		MissionUpdate(missionBlockCount, blockCount);
	}
	else {
		// ミッションフレーム
		UIs_[kUIIndexMissionFrame]->Update();
		// ミッションテキスト
		UIs_[kUIIndexMissionText]->Update();

		// ミッションブロック更新
		MissionBlockCountUpdate(missionBlockCount);

		// 持っているブロックの数更新
		BlockCountUpdate(blockCount);

	}

	// ミッション2フレーム
	UIs_[kUIIndexMission2Frame]->Update();

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

	// ミッションフレーム
	UIs_[kUIIndexMissionFrame] = std::make_unique<UIMissionFrame>();
	UIs_[kUIIndexMissionFrame]->Initialize(textureHandles_[kUITextureHandleIndexMissionFrame], "UIMissionFrame", missionFrameSize, leftTop);

	// ミッションテキスト
	UIs_[kUIIndexMissionText] = std::make_unique<UIMissionText>();
	UIs_[kUIIndexMissionText]->Initialize(textureHandles_[kUITextureHandleIndexMissionText], "UIMissionText", missionTextSize, leftTop);

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

	// ミッションクリア
	UIs_[kUIIndexMissionClear] = std::make_unique<UIMissionClear>();
	UIs_[kUIIndexMissionClear]->Initialize(textureHandles_[kUITextureHandleIndexMissionClear], "UIMissionClear", missionClearSize, leftTop);
	UIs_[kUIIndexMissionClear]->SetIsInvisible(true);

	// ミッション2フレーム
	UIs_[kUIIndexMission2Frame] = std::make_unique<UIMissionFrame>();
	UIs_[kUIIndexMission2Frame]->Initialize(textureHandles_[kUITextureHandleIndexMissionFrame], "UIMission2Frame", missionFrameSize, leftTop);


}

void UIManager::MissionUpdate(uint32_t missionBlockCount, uint32_t blockCount)
{

	Vector2 leftTop = { 0.0f, 0.0f };

	// 持っているブロックの数更新
	BlockCountUpdate(blockCount);

	// クリア
	if (!missionBeenUpdateFadeIn_) {

		if (!IsStamped_) {
			// ハンコ
			Stamp();
		}
		else {
			missionBeenUpdateColor_.w -= 0.05f;
			if (missionBeenUpdateColor_.w <= 0.0f) {
				missionBeenUpdateColor_.w = 0.0f;
				missionBeenUpdateFadeIn_ = true;
				UIs_[kUIIndexMissionClear]->SetIsInvisible(true);
				// ミッションブロック更新
				MissionBlockCountUpdate(missionBlockCount);
				audioManager_->PlayWave(kGameAudioNameIndexMissionOccurrrence);
			}

			ClearMissionUpdate((1.0f - missionBeenUpdateColor_.w));
		}

	}
	// 新しいものが飛んでくる
	else {
		missionBeenUpdateColor_.w += 0.05f;
		if (missionBeenUpdateColor_.w >= 1.0f) {
			missionBeenUpdateColor_.w = 1.0f;
			missionBeenUpdateFadeIn_ = false;
			missionBeenUpdate_ = false;
			IsStamped_ = false;

			// ミッションクリア
			UIs_[kUIIndexMissionClear]->SetPosition(UIInitPositions_[kUIIndexMissionClear]);

		}

		NewMissionMove(missionBeenUpdateColor_.w);

	}

	// ミッションフレーム
	UIs_[kUIIndexMissionFrame]->SetColor(missionBeenUpdateColor_);
	// ミッションテキスト
	UIs_[kUIIndexMissionText]->SetColor(missionBeenUpdateColor_);

	// ミッション番号10の位
	UIs_[kUIIndexMissionNumTensPlace]->SetColor(missionBeenUpdateColor_);
	// ミッション番号1の位
	UIs_[kUIIndexMissionNumOnesPlace]->SetColor(missionBeenUpdateColor_);
	// ミッション分母10の位
	UIs_[kUIIndexMissionDenominatorTensPlace]->SetColor(missionBeenUpdateColor_);
	// ミッション分母1の位
	UIs_[kUIIndexMissionDenominatorOnesPlace]->SetColor(missionBeenUpdateColor_);
	// ミッション分子10の位
	UIs_[kUIIndexMissionNumeratorTensPlace]->SetColor(missionBeenUpdateColor_);
	// ミッション分子1の位
	UIs_[kUIIndexMissionNumeratorOnesPlace]->SetColor(missionBeenUpdateColor_);

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

void UIManager::NewMissionMove(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addStart = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionFrame];
	end = UIInitPositions_[kUIIndexMissionFrame];
	start.x += addStart;
	UIs_[kUIIndexMissionFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionText];
	end = UIInitPositions_[kUIIndexMissionText];
	start.x += addStart;
	UIs_[kUIIndexMissionText]->SetPosition(Ease::Easing(Ease::EaseName::EaseInCubic, start, end, t));
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

void UIManager::ClearMissionUpdate(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addEnd = 520.0f;

	// ミッションフレーム
	start = UIInitPositions_[kUIIndexMissionFrame];
	end = UIInitPositions_[kUIIndexMissionFrame];
	end.x += addEnd;
	UIs_[kUIIndexMissionFrame]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kUIIndexMissionText];
	end = UIInitPositions_[kUIIndexMissionText];
	end.x += addEnd;
	UIs_[kUIIndexMissionText]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
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
	start = UIInitPositions_[kUIIndexMissionClear];
	end = UIInitPositions_[kUIIndexMissionClear];
	end.x += addEnd;
	UIs_[kUIIndexMissionClear]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

}

void UIManager::Stamp()
{

	float speed = 0.05f;

	stampT_ += speed;
	if (stampT_ >= 1.0f) {
		stampT_ = 1.0f;
		if (stampCooltime_ >= 1.0f) {
			stampCooltime_ = 0.0f;
			IsStamped_ = true;
		}
		else {
			stampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 120.0f, 105.0f };
	Vector2 end = { 96.0f, 84.0f };
	UIs_[kUIIndexMissionClear]->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, stampT_));

}
