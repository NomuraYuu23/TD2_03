#pragma once

#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Matrix4x4.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 衝突
	/// </summary>
	void AllCollision();

private: // メンバ関数

private: // メンバ変数

	// 計算
	Vector3Calc* v3Calc;
	Matrix4x4Calc* m4Calc;

	// ゲームオブジェクト

};
