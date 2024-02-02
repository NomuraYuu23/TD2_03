#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"

class Rocket
{

public:

	void Initialize(Model* model);

	void Update();

	void Draw(BaseCamera camera);

private:

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	void ApplyGlobalVariables();

public:

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	inline float GetToPlayerLength() { return toPlayerLength_; };

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_;

	// プレイヤーとの最短距離
	float toPlayerLength_;

};

