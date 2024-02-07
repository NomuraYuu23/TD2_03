#pragma once
//#include "Input.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Input/input.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../Magnet/Magnet.h"

#include "PlayerAnimation/PlayerAnimation.h"

#include <vector>
#include <optional>
#include "../AudioManager/GameAudioManager.h"
#include "../Rocket/Rocket.h"

class Block;
class Screw;

class Player{
public:
	
	enum class Behavior {
		kRoot,
		kAttack,
		kDrop,
	};


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<std::unique_ptr<Model>, PlayerPartIndex::kPlayerPartIndexOfCount>& models);
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	void BehaviorDropInitialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Block* block,size_t blockNum);
	void BehaviorRootUpdate(Block* block, size_t blockNum);
	void BehaviorAttackUpdate();
	void BehaviorDropUpdate();
	Behavior GetBehavior() { return behavior_; };
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(Model* model, BaseCamera& camera);
	void InitializeFloatingGimmick();

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };
	inline void SetViewProjection(BaseCamera& camera) {
		camera_ = &camera;
	};
	

	//void OnCollision(WorldTransform& parent);
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData) ;
	
	void SetTarget(WorldTransform* target) { target_ = target; };
	//void SetParticle(Particle* particle) { particle_ = particle; };

	void SetScrew(std::list<std::unique_ptr<Screw>>* s) { screws_ = s; };

	OBB* GetCollider() { return collider_.get(); };
	Magnet* GetMagnet() { return magnet_.get(); };
	Vector3 GetDirection() { return direction_; };
	void SetCircle(Model* m) { modelCircle_ = m; };

	// アニメーション
	PlayerAnimationIndex GetPlayerAnimationNo() { return playerAnimationNo_; }

	// ブロック
	Block* GetBlock() { return block_; }

	// オーディオマネージャー
	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

	bool isOutGameArea();
	bool GetIsRideConnectFlooar() { return isRideConnectFlooar_; };
	void SetIsRideConnectFlooar(bool is) {isRideConnectFlooar_=is; };
	PlayerAnimation* GetAnimation() { return playerAnimation_.get(); };

	void CollisionWithRocket();
	void SetRocket(Rocket* rocket) { rocket_ = rocket; }
	int32_t GetControlLength() { return controlLength_; };
	void SetIsCanShot(bool is) { isCanShot_ = is; };
	void SetIsCanGravity(bool is) { isCanGravity_ = is; };
	bool GetIsUsedGravity() { return isUsedGravity_; };
	void SetIsCanLockOn(bool is) { isCanLockOn_ = is; };

	int32_t GetNotFallTime() { return notFallTime_; }
	int32_t GetNotFallLength() { return notFallLength_; }
	void MagnetSizeUp() { sizeUpTime_ = sizeUpLength_; };
private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;

	Input* input_ = nullptr;

	//uint16_t period = 120;
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	bool isFlooar_ = false;
	bool isRideConnectFlooar_=false;

	//調整用
	uint32_t attackFrame_ = 55;
	uint32_t attackFrameCount_ = 0u;

	//向き
	Vector3 direction_;
	Matrix4x4 directionMatrix_;
	
	WorldTransform* target_;
	BaseCamera* camera_;

	//ジャンプ用変数
	Vector3 velocity_;
	Vector3 acceleration_;
	Vector3 kGravity;
	Vector3 kJumpVelocity;

	std::list<std::unique_ptr<Screw>>* screws_;
	std::unique_ptr<OBB> collider_;
	std::unique_ptr<Magnet> magnet_;
	std::unique_ptr <Material> mat_;
	//float magnetRadius_ = 12.0f;

	Model* modelCircle_;
	std::unique_ptr <Material> materialCircle_;
	WorldTransform worldTransformCircle_;
	float magnetRadius_ = 12.0f;

	// 重力
	uint32_t gravityFrame_ = 20;
	uint32_t gravityFrameCount_ = 0u;

	// アニメーション
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> models_;
	std::unique_ptr<PlayerAnimation> playerAnimation_;
	PlayerAnimationIndex playerAnimationNo_;

	// ブロック
	Block* block_ = nullptr;

	//刺すブロックのネジ穴の番地
	size_t blockNum_;
	//掴んでるネジのポインタ、リリース用
	Screw* holdScrew_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

	// 歩行速度
	float characterSpeed_ = 0.3f;

	// ロケット
	Rocket* rocket_;
	//移動を押してる長さ(累計)
	int32_t controlLength_;
	bool isCanShot_;//チュートリアル用ネジ撃ち出来るかどうか
	bool isCanGravity_;
	bool isUsedGravity_=false;
	bool isCanLockOn_ = true;

	Vector3 prePosition_;
	bool isLastBlockConnect_=false;
	int32_t notFallTime_;
	int32_t notFallLength_=30;//端で落ちないように踏ん張る長さ
	bool isNotFall = true;
	int32_t sizeUpTime_;//サイズが大きくなってる時間
	int32_t sizeUpLength_=0;//大きくなる時間の長さ
	//float magnetBigRadius_;//大きい間のサイズ
};
