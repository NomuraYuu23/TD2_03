#pragma once
//#include "Input.h"
#include "../Engine/3D/Model.h"
#include "../Engine/3D/Material.h"
#include "../Engine/3D/WorldTransform.h"
#include "../Engine/Input/input.h"
#include "../Engine/Camera/BaseCamera.h"
#include <vector>
#include <optional>

class Player{
public:
	struct HierarchicalAnimation {
		Model* model_;
		WorldTransform worldTransform_;
	};
	
	enum class AttackBehavior {
		kPre,
		kAttack,
		kEnd,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<HierarchicalAnimation>& models);
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	void BehaviorDashInitialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(BaseCamera& camera);
	void InitializeFloatingGimmick();


	/*
		inline WorldTransform* GetWorldTransform() { return &worldTransform_; };
		*/
	inline void SetViewProjection(BaseCamera& camera) {
		camera_ = &camera;
	};
	inline void SetWepon(Model* model) { modelWepon_ = model; };

	

	void OnCollision(WorldTransform& parent);

	
	void SetTarget(WorldTransform* target) { target_ = target; };
	//void SetParticle(Particle* particle) { particle_ = particle; };
private:
	WorldTransform worldTransform_;
	std::vector<HierarchicalAnimation> models_;
	//WorldTransform worldTransform_;
//Model* model_ = nullptr;
	//Model* modelBody_  = nullptr;
	//Model* modelHead_  = nullptr;
	//Model* modelL_arm_ = nullptr;
	//Model* modelR_arm_ = nullptr;
	//std::vector<Model*> models_;
	//std::vector <HierarchicalAnimation> models_;
	WorldTransform worldTransformWepon_;
	Model* modelWepon_;

	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;

	Input* input_ = nullptr;

	//uint16_t period = 120;
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	AttackBehavior attackBehavior_ = AttackBehavior::kPre;

	bool isFlooar_ = false;

	//調整用
	int frameCount_ = 0;

	//向き
	Vector3 direction_;
	Matrix4x4 directionMatrix_;

	

	XINPUT_STATE joyState_;
	//1フレーム前の入力情報
	XINPUT_STATE preJoyState_;

	
	WorldTransform* target_;
	BaseCamera* camera_;
};
