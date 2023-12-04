#pragma once
#include "../Collider.h"

class Triangle : public Collider
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3);

	void SetVertices(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	Vector3 vertices_[3];

};

