#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"

class AreaOut
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// チカチカ
	/// </summary>
	void Flickering();

	/// <summary>
	/// UV移動
	/// </summary>
	void UvMove();

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> material_ = nullptr;

	// α
	float alpha_ = 0.0f;

	// 移動
	float move_ = 0.0f;

	// 見えるはんい
	float invisibleLimit_ = 130.0f;

	// 見えない
	bool isInvisible_ = false;

};

