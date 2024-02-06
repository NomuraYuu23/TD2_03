#pragma once
#include <array>
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Math/Ease.h"

class Player;

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
	kPlayerAnimationIndexFalling, // 落下状態
	kPlayerAnimationIndexLockOn, // ロックオン状態
	kPlayerAnimationIndexLikelyToFall, // 落ちそうなアニメーション
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
		float blockAddPositionY_ = 2.0f;

	};

	// 落下状態
	struct WorkFalling {
		float speed_ = 0.02f;
		float parameter_ = 0.0f;
		Vector3 startRotate_ = { 0.0f,0.0f,0.0f };
		Vector3 endRotate_ = { 3.14f,0.0f,0.0f };
	};

	// ロックオン
	struct WorkLockOn{
		float rotateX_ = -0.7f;
		float shake_ = 0.02f;
	};

	// 落ちそうなアニメーション
	struct WorkLikelyToFall{
		uint32_t frame_ = 30;
		uint32_t frameCount_ = 0u;
		uint32_t phaseNum_ = 0;
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> currentTransforms_ = {};
		std::array<TransformStructure, PlayerPartIndex::kPlayerPartIndexOfCount> nextTransforms_ = {};
		Ease::EaseName easeName_ = Ease::EaseName::Lerp;
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

private: // 落ちそう状態

	enum LikelyToFallPhaseIndex {
		kScrewThrowingPhaseIndexTilt1, // 傾く1
		kScrewThrowingPhaseIndexReturn1, // 戻る1
		kScrewThrowingPhaseIndexTilt2, // 傾く2
		kScrewThrowingPhaseIndexReturn2, // 戻る2
		kLikelyToFallPhaseIndexOfCount // 数
	};
	static std::array<AnimationData, LikelyToFallPhaseIndex::kLikelyToFallPhaseIndexOfCount> likelyToFallAnimationData_;


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
		"Falling",
		"LockOn",
		"LikelyToFall",
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

	const std::array <std::string, LikelyToFallPhaseIndex::kLikelyToFallPhaseIndexOfCount> kLikelyToFallPhaseIndexNames_ = {
		"Tilt1",
		"Return1",
		"Tilt2",
		"Return2",
	};

public:

	void Initialize(Player* player);

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
	// 磁石部分の例外
	void ScrewThrowingMagunetException();

	// 落下状態
	void FallingInitialize();
	void FallingUpdate();

	// ロックオン状態
	void LockOnInitialize();
	void LockOnUpdate();

	// 落ちそう状態
	void LikelyToFallInitialize();
	void LikelyToFallUpdate();

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

	uint32_t GetScrewThrowPhase() { return workScrewThrowing_.phaseNum_; };

private:

	// ワールドトランスフォーム
	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount> worldTransforms_;

	Player* player_;

	// アニメーションタイム
	std::array<float, PlayerPartIndex::kPlayerPartIndexOfCount> times_;

	PlayerAnimationIndex prevPlayerAnimationNo_;

	WorkStand workStand_;

	WorkWalk workWalk_;

	WorkGravity workGravity_;

	WorkScrewThrowing workScrewThrowing_;

	WorkFalling workFalling_;

	WorkLockOn workLockOn_;

	WorkLikelyToFall workLikelyToFall_;

};

