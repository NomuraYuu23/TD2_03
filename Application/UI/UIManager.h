#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "UI.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexScrews, // ねじ
	kUITextureHandleIndexNumber, // 数字
	kUITextureHandleIndexSymbol, // 記号
	kUITextureHandleIndexFrame, // フレーム
	kUITextureHandleIndexEnergyPlate, // エナジープレート
	kUITextureHandleIndexEnergy, // エナジー
	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexFrame, // フレーム
	kUIIndexRemainingScrewsIcon, // 残りねじアイコン
	kUIIndexRemainingScrewsCross, // 残りねじのx
	kUIIndexRemainingScrewsTensPlace, // 残りねじの数字10の位
	kUIIndexRemainingScrewsOnesPlace, // 残りねじの数字1の位
	kUIIndexTimerMinutes, // タイマー分
	kUIIndexTimerSecondsTensPlace, // タイマー秒10の位
	kUIIndexTimerSecondsOnesPlace, // タイマー秒1の位
	kUIIndexTimerColon, // タイマーコロン
	kUIIndexEnergy, // エナジー
	kUIIndexEnergyPlate, // エナジープレート
	kUIIndexOfCount
};

class UIManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles">テクスチャハンドル</param>
	/// <param name="energyMax">エナジー最大量</param>
	void Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles, float energyMax);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="screwCount">ねじの数</param>
	/// <param name="energy">エナジー</param>
	void Update(uint32_t screwCount, float energy);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// UIごとの初期化
	/// </summary>
	/// <param name="energyMax">エナジー最大量</param>
	void UIInitialize(float energyMax);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

};

