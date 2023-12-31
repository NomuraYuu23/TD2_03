#pragma once
//#include "Input.h"
#include "../Engine/3D/Model.h"
#include "../Engine/3D/Material.h"
#include "../Engine/3D/WorldTransform.h"
#include "../Engine/Input/input.h"
#include "../Engine/Camera/BaseCamera.h"
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
	
	};


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Block* block,size_t blockNum);
	void BehaviorRootUpdate(Block* block, size_t blockNum);
	void BehaviorAttackUpdate();
	
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
	

	void OnCollision(WorldTransform& parent);

	
	void SetTarget(WorldTransform* target) { target_ = target; };
	//void SetParticle(Particle* particle) { particle_ = particle; };

	void SetScrew(std::vector<std::unique_ptr<Screw>>* s) { screws_ = s; };
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

	std::vector<std::unique_ptr<Screw>>* screws_;
};
