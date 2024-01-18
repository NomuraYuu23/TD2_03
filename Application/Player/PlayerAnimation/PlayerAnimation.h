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

enum PlayerAnimationIndex {
	kPlayerAnimationIndexStand, // 立ち状態
	kPlayerAnimationIndexWalk, // 歩き状態
	kPlayerAnimationIndexOfCount // 数
};

class PlayerAnimation
{

public: // サブクラス

	// 立ち状態
	struct WorkStand {

	};

	// 歩き状態
	struct WorkWalk {
		float rotate_ = 0.0f;
		float speed_ = 0.1f;
		int32_t angle_ = 0;
	};

public:

	void Initialize(WorldTransform* worldTransform);

	void Update(PlayerAnimationIndex playerAnimationNo);

	void Draw(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, BaseCamera& camera, Material* mat);

	void ImGuiDraw();

private:
	
	// 立ち状態
	void StandInitialize();
	void StandUpdate();
	
	// 歩き状態
	void WalkInitialize();
	void WalkUpdate();

public:

	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount>* GetWorldTransforms() { return &worldTransforms_; }

private:

	// ワールドトランスフォーム
	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount> worldTransforms_;

	WorldTransform* playerWorldTransform_;

	// アニメーションタイム
	std::array<float, PlayerPartIndex::kPlayerPartIndexOfCount> times_;

	PlayerAnimationIndex prevPlayerAnimationNo_;

	WorkStand workStand_;

	WorkWalk workWalk_;

};

