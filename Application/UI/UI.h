#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "../../Engine/2D/Sprite.h"

enum UITextureHandleIndex {

	kUITextureHandleIndexOfCount
};

enum UISpriteIndex {

	kUISpriteIndexOfCount
};

class UI
{

public: // サブクラス

	struct UI{
		std::unique_ptr<Sprite> sprite_;
	};

public: // メンバ関数

	void Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// スプライト
	std::array<std::unique_ptr<Sprite>, UISpriteIndex::kUISpriteIndexOfCount> sprites_;

};

