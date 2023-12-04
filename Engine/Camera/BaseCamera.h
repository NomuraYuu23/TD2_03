#pragma once
#include "../3D/ViewProjection.h"
class BaseCamera
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ビュープロジェクションゲッター
	/// </summary>
	/// <returns></returns>
	ViewProjection GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ビュープロジェクションアドレスゲッター
	/// </summary>
	/// <returns></returns>
	ViewProjection* GetViewProjectionAddress() { return &viewProjection_; }

	Matrix4x4 GetMatrix() { return matrix_; }

protected:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	// カメラ行列
	Matrix4x4 matrix_;

};

