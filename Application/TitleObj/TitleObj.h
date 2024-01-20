#pragma once
#include <array>
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"

class TitleObj
{

public:

	void Initialize(Model* model, const std::string& objName);

	void Update();

	void Draw(BaseCamera& camera);

private: // 関数

	/// <summary>
	/// ふわふわ
	/// </summary>
	void Fluffy();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

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
	Model* model_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	
	// 移動する位置
	std::array<Vector3, 2> position_;

	// 移動用媒介変数
	float moveT_;

	// 移動用媒介変数速度
	float moveTSpeed_;

	// 移動用媒介変数は増えるか
	bool itIncreaseMoveT_;
	
	// 回転速度
	Vector3 rotateSpeed_;
	
	// 大きさ
	Vector3 size_;

	// ネーム
	std::string objName_;

};

