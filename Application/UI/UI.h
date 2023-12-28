#pragma once
#include <array>
#include <memory>
#include <cstdint>

enum UITextureHandleIndex {

	kUITextureHandleIndexOfCount
};

enum UISpriteIndex {

	kUISpriteIndexOfCount
};

class UI
{

public: // メンバ関数

	void Initialize();

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;


};

