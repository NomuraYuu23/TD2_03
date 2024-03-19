#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collision/CollisionData.h"
class Planet
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(BaseCamera& camera);
	void DrawFlag(BaseCamera& camera);
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	OBB* GetCollider() { return collider_.get(); };
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData);
	void SetFlagModel(Model* m) { modelFlag_ = m; };
	void SetPosition(const Vector3& pos) { position_ = pos; };
	void SetFlagRotateY(float y) { worldTransformFlag_.transform_.rotate.y = y; };
	WorldTransform* GetWorldTransform() { return &worldTransform_; };
	void SetFlagTexturehandle(uint32_t handle) { flagTextureHandle_ = handle; };

	void LifeExpectancyAnnounced() { flagLifeTime_ = flagrRmainingLife; }
	uint32_t GetFlagLifeTime() { return flagLifeTime_; }

	void SetCollisionTextureHandle(uint32_t handle) { collisionTextureHandle_ = handle; }
	void SetTextureHandle(uint32_t handle) { textureHandle_ = handle; };
private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformFlag_;
	// モデル
	Model* model_ = nullptr;
	Model* modelFlag_;
	//マテリアル
	std::unique_ptr<Material> material_ = nullptr;

	// 回転速度
	float rotateSpeed_ = 0.001f;

	// 位置
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };

	float size_ = 1.0f;
	std::unique_ptr<OBB> collider_;
	uint32_t flagTextureHandle_;

	// フラッグの余命
	uint32_t flagLifeTime_;
	uint32_t flagrRmainingLife = 30;

	// 惑星の色
	uint32_t textureHandle_;
	uint32_t collisionTextureHandle_;

	// つながってるか
	bool isConnect = false;

};

