#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "UI.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexScrews, // ねじ
	kUITextureHandleIndexNumber, // 数字
	UITextureHandleIndexSymbol, // 記号
	kUITextureHandleIndexOfCount
};

enum UIIndex {
	
	kUIIndexRemainingScrewsIcon, // 残りねじアイコン
	//kUIIndexRemainingScrewsCross, // 残りねじのx
	//kUIIndexRemainingScrewsTensPlace, // 残りねじの数字10の位
	//kUIIndexRemainingScrewsOnesPlace, // 残りねじの数字1の位
	//kUIIndexTimerMinutes, // タイマー分
	//kUIIndexTimerSecondsTensPlace, // タイマー秒10の位
	//kUIIndexTimerSecondsOnesPlace, // タイマー秒1の位
	//kUIIndexTimerColon, // タイマーコロン
	kUIIndexOfCount
};

class UIManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles">テクスチャハンドル</param>
	void Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// UIごとの初期化
	/// </summary>
	void UIInitialize();

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

};

