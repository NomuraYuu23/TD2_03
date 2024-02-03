#pragma once

#include "../../Engine/2D/Sprite.h"
#include "../Screw/Screw.h"
#include <array>

enum WarningDrawTextureHandleIndex {
	kWarningDrawTextureHandleIndexWarning,
	kWarningDrawTextureHandleIndexOfCount
};

enum WarningDrawUIIndex {
	kWarningDrawUIIndexLeftWarning,
	kWarningDrawUIIndexRightWarning,
	kWarningDrawUIIndexOfCount
};

class WarningDraw
{

public:

	void Initialize(const std::array<uint32_t, WarningDrawTextureHandleIndex::kWarningDrawTextureHandleIndexOfCount>& textureHandles);

	void Update(const Matrix4x4& viewProjection, std::vector<Screw*> screws);

	void Draw();

private:

	void UIInitialize();

private:

	// テクスチャハンドル
	std::array<uint32_t, WarningDrawTextureHandleIndex::kWarningDrawTextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<Sprite>, WarningDrawUIIndex::kWarningDrawUIIndexOfCount> UIs_;

	// α
	float alpha = 0.0f;

};
