#include "UIManager.h"
#include "UIScrew/UIScrew.h"
#include "UINumber/UINumber.h"
#include "UISymbol/UISymbol.h"
#include "UIFrame/UIFrame.h"
#include "UIEnergyPlate/UIEnergyPlate.h"
#include "UIEnergy/UIEnergy.h"
#include "UIMissionFrame/UIMissionFrame.h"
#include "UIMissionText/UIMissionText.h"

void UIManager::Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

}

void UIManager::Update(uint32_t screwCount, uint32_t missionBlockCount, uint32_t blockCount)
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
	leftTop = { 0.0f, 0.0f };
	UIs_[kUIIndexTimerMinutes]->Update(leftTop);
	// タイマー秒10の位
	UIs_[kUIIndexTimerSecondsTensPlace]->Update(leftTop);
	// タイマー秒1の位
	UIs_[kUIIndexTimerSecondsOnesPlace]->Update(leftTop);
	// タイマーコロン
	UIs_[kUIIndexTimerColon]->Update();
	// ミッションフレーム
	UIs_[kUIIndexMissionFrame]->Update();
	// ミッションテキスト
	UIs_[kUIIndexMissionText]->Update();

	// ミッション番号10の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount / 10);
	UIs_[kUIIndexMissionNumTensPlace]->Update(leftTop);
	// ミッション番号1の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount % 10);
	UIs_[kUIIndexMissionNumOnesPlace]->Update(leftTop);

	// ミッション分母10の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount / 10);
	UIs_[kUIIndexMissionDenominatorTensPlace]->Update(leftTop);
	// ミッション分母1の位
	leftTop.x = 128.0f * static_cast<float>(missionBlockCount % 10);
	UIs_[kUIIndexMissionDenominatorOnesPlace]->Update(leftTop);
	// ミッション分子10の位
	leftTop.x = 128.0f * static_cast<float>(blockCount / 10);
	UIs_[kUIIndexMissionNumeratorTensPlace]->Update(leftTop);
	// ミッション分子1の位
	leftTop.x = 128.0f * static_cast<float>(blockCount % 10);
	UIs_[kUIIndexMissionNumeratorOnesPlace]->Update(leftTop);

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

	// タイマー秒1の位
	UIs_[kUIIndexMissionNumTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionNumTensPlace", numberSize, leftTop);

	// タイマー秒10の位
	UIs_[kUIIndexMissionNumOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionNumOnesPlace", numberSize, leftTop);


	// ミッション分母10の位
	UIs_[kUIIndexMissionDenominatorTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionDenominatorTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionDenominatorTensPlace", numberSize, leftTop);
	// ミッション分母1の位
	UIs_[kUIIndexMissionDenominatorOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionDenominatorOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionDenominatorOnesPlace", numberSize, leftTop);
	// ミッション分子10の位
	UIs_[kUIIndexMissionNumeratorTensPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumeratorTensPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionNumeratorTensPlace", numberSize, leftTop);
	// ミッション分子1の位
	UIs_[kUIIndexMissionNumeratorOnesPlace] = std::make_unique<UINumber>();
	UIs_[kUIIndexMissionNumeratorOnesPlace]->Initialize(textureHandles_[kUITextureHandleIndexNumber], "UIMissionNumeratorOnesPlace", numberSize, leftTop);

}
