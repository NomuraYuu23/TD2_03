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

#include <vector>
#include <optional>

class Block;
class Screw;

class Player{
public:
	struct HierarchicalAnimation {
		Model* model_;
		WorldTransform worldTransform_;
	};
	
	enum class Behavior {
		kRoot,
		kAttack,
		kDrop,
	};


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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
private:
	WorldTransform worldTransform_;
	std::vector<HierarchicalAnimation> models_;


	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;

	Input* input_ = nullptr;

	//uint16_t period = 120;
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	bool isFlooar_ = false;

	//調整用
	int frameCount_ = 0;

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
};
