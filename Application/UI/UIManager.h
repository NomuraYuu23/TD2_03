#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "UI.h"

enum UITextureHandleIndex {

	kUITextureHandleIndexOfCount
};

enum UIIndex {
	
	kRemainingScrewsIcon, // 残りねじアイコン
	kRemainingScrewsCross, // 残りねじのx
	kRemainingScrewsTensPlace, // 残りねじの数字10の位
	kRemainingScrewsOnesPlace, // 残りねじの数字1の位
	kTimerMinutes, // タイマー分
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

