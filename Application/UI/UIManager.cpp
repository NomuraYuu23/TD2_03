#include "UIManager.h"
#include "UIScrew/UIScrew.h"
#include "UINumber/UINumber.h"
#include "UISymbol/UISymbol.h"
#include "UIFrame/UIFrame.h"
#include "UIEnergyPlate/UIEnergyPlate.h"
#include "UIEnergy/UIEnergy.h"

void UIManager::Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles, float energyMax)
{

	textureHandles_ = textureHandles;

	UIInitialize(energyMax);

}

void UIManager::Update(uint32_t screwCount, float energy)
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
	//エナジー
	static_cast<UIEnergy*>(UIs_[kUIIndexEnergy].get())->Update(energy);
	//エナジープレート
	UIs_[kUIIndexEnergyPlate]->Update();

}

void UIManager::Draw()
{

	for (uint32_t i = 0;i < UIIndex::kUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}

}

void UIManager::UIInitialize(float energyMax)
{

	Vector2 leftTop = { 0.0f, 0.0f };
	Vector2 numberSize = { 128.0f, 128.0f };
	Vector2 symbolSize = { 192.0f, 192.0f };
	Vector2 frameSize = { 1280.0f, 720.0f };
	Vector2 EnergySize = { 1536.0f, 96.0f };
	Vector2 EnergyPlateSize = { 500.0f, 120.0f};

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

	// エナジー
	leftTop = { 0.0f, 0.0f };
	UIs_[kUIIndexEnergy] = std::make_unique<UIEnergy>();
	static_cast<UIEnergy*>(UIs_[kUIIndexEnergy].get())->Initialize(textureHandles_[kUITextureHandleIndexEnergy], "UIIndexEnergy", EnergySize, leftTop, energyMax);

	// エナジープレート
	leftTop = { 0.0f, 0.0f };
	UIs_[kUIIndexEnergyPlate] = std::make_unique<UIEnergyPlate>();
	UIs_[kUIIndexEnergyPlate]->Initialize(textureHandles_[kUITextureHandleIndexEnergyPlate], "UIIndexEnergyPlate", EnergyPlateSize, leftTop);

}
