#include "BaseCamera.h"

void BaseCamera::Initialize()
{

	//ビュープロジェクション
	viewProjection_.Initialize();

	// カメラ行列
	matrix_ = Matrix4x4Calc::GetInstance()->MakeAffineMatrix(viewProjection_.transform_.scale, viewProjection_.transform_.rotate, viewProjection_.transform_.translate);

}

void BaseCamera::Update()
{

	//ビュープロジェクション
	viewProjection_.UpdateMatrix();

	// カメラ行列
	matrix_ = Matrix4x4Calc::GetInstance()->MakeAffineMatrix(viewProjection_.transform_.scale, viewProjection_.transform_.rotate, viewProjection_.transform_.translate);

}
