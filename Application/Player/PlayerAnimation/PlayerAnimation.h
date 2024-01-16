#pragma once
#include <array>
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/3D/Model.h"

enum PlayerPartIndex {
	kPlayerPartIndexBody, // 体
	kPlayerPartIndexLeftLeg, // 左足
	kPlayerPartIndexRightLeg, // 右足
	kPlayerPartIndexMagnet, // 磁石(頭)
	kPlayerPartIndexOfCount // 数
};

class PlayerAnimation
{

public:

	void Initialize(WorldTransform* worldTransform);

	void Update();

	void Draw(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, BaseCamera& camera, Material* mat);

public:

	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount>* GetWorldTransforms() { return &worldTransforms_; }

private:

	// ワールドトランスフォーム
	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount> worldTransforms_;

	WorldTransform* playerWorldTransform_;

	// アニメーションタイム
	std::array<float, PlayerPartIndex::kPlayerPartIndexOfCount> times_;

};

