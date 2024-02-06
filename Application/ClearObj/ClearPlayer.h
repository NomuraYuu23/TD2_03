#pragma once
#include <array>
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../Player/PlayerAnimation/PlayerAnimation.h"

class ClearPlayer
{

public:

	void Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, const std::string& objName);

	void Update();

	void Draw(BaseCamera& camera);

private: // 関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summa
	void ApplyGlobalVariables();

private: // 変数

	// モデル
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> models_;

	// ワールドトランスフォーム
	std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount> worldTransforms_;

	// 回転
	Vector3 rotate_;

	// 回転パラメータ
	float rotateParameter_;

	// 回転パラメータ速度
	float rotateParameterSpeed_;

	// 最大回転
	float rotateMax_ = 1.0f;;

	// 大きさ
	Vector3 size_;

	// 位置
	Vector3 position_;

	// ネーム
	std::string objName_;

};

