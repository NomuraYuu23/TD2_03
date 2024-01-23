#include "SparkParticle.h"

void SparkParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f, 0.0f, 0.0f };

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	lifeTime_ = 1.0f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	billBoardName_ = kBillBoardNameIndexAllAxis;

	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

}

void SparkParticle::Update(const Matrix4x4& billBoardMatrix)
{
}
