#pragma once
#include <array>
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Math/Ease.h"

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
	kPlayerAnimationIndexGravity, // 重力状態
	kPlayerAnimationIndexScrewThrowing, // ねじ投擲状態
	kPlayerAnimationIndexOfCount // 数
};

class PlayerAnimation
{

public: // サブクラス

	struct AnimationData{
		TransformStructure transforms_[PlayerPartIndex::kPlayerPartIndexOfCount];
		uint32_t frame_ = 10;
		Ease::EaseName easeName_ = Ease::EaseName::Lerp;
	};

	// 立ち状態
	struct WorkStand {

	};

	// 歩き状態
	struct WorkWalk {
		float rotate_ = 0.0f;
		float speed_ = 0.1f;
		int32_t angle_ = 0;
	};

	// 重力状態
	struct WorkGravity {
		uint32_t frame_ = 20;
		uint32_t frameCount_ = 0u;
		uint32_t phaseNum_ = 0;
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> currentTransforms_ = {};
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> nextTransforms_ = {};
		Ease::EaseName easeName_ = Ease::EaseName::Lerp;
	};

	// ねじ投擲状態
	struct WorkScrewThrowing {
		uint32_t frame_ = 20;
		uint32_t frameCount_ = 0u;
		uint32_t phaseNum_ = 0;
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> currentTransforms_ = {};
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> nextTransforms_ = {};
		Ease::EaseName easeName_ = Ease::EaseName::Lerp;

		// ブロック
		Vector3 blockPosition_ = {};

	};

private: // 重力状態

	enum GravityPhaseIndex {
		kGravityPhaseIndexExtend1, //のびる1
		kGravityPhaseIndexCollapse, //つぶれる
		kGravityPhaseIndexExtend2, //のびる2
		kGravityPhaseIndexReturn, //戻る
		kGravityPhaseIndexOfCount // 数
	};
	static std::array<AnimationData, GravityPhaseIndex::kGravityPhaseIndexOfCount> gravityAnimationData_;

private: // ねじ投擲状態

	enum ScrewThrowingPhaseIndex {
		kScrewThrowingPhaseIndexWarp1, // からだを反らす1
		kScrewThrowingPhaseIndexThrowing, // ねじ投擲する
		kScrewThrowingPhaseIndexTurn, // ぐるぐる
		kScrewThrowingPhaseIndexWarp2, // からだを反らす2
		kScrewThrowingPhaseIndexReturn, //戻る
		kScrewThrowingPhaseIndexOfCount // 数
	};
	static std::array<AnimationData, ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount> screwThrowingAnimationData_;

private: // 文字列

	const std::array <std::string, PlayerPartIndex::kPlayerPartIndexOfCount> kPlayerPartIndexNames_ = {
		"Body",
		"LeftLeg",
		"RightLeg",
		"Magnet",
	};

	const std::array <std::string, PlayerAnimationIndex::kPlayerAnimationIndexOfCount> kPlayerAnimationIndexNames_ = {
		"Stand",
		"Walk",
		"Gravity",
		"ScrewThrowing",
	};
	
	const std::array <std::string, GravityPhaseIndex::kGravityPhaseIndexOfCount> kGravityPhaseIndexNames_ = {
		"Extend1",
		"Collapse",
		"Extend2",
		"Return",
	};

	const std::array <std::string, ScrewThrowingPhaseIndex::kScrewThrowingPhaseIndexOfCount> kScrewThrowingPhaseIndexNames_ = {
		"Warp1",
		"Throwing",
		"Turn",
		"Warp2",
		"Return",
	};

public:

	void Initialize(WorldTransform* worldTransform);

	void Update(PlayerAnimationIndex playerAnimationNo);

	void Draw(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, BaseCamera& camera, Material* mat);

	void ImGuiDraw();

private:
	
	// アニメーション初期化
	void AnimationInitialize();

	// 初期モデル位置
	void TransformInitialize();

	// 立ち状態
	void StandInitialize();
	void StandUpdate();
	
	// 歩き状態
	void WalkInitialize();
	void WalkUpdate();

	// 重力状態
	void GravityInitialize();
	void GravityUpdate();

	// ねじ投擲状態
	void ScrewThrowingInitialize();
	void ScrewThrowingUpdate();

private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

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

	WorkGravity workGravity_;

	WorkScrewThrowing workScrewThrowing_;

};

