#include "SparkParticle.h"
#include <random>

void SparkParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

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

	Vector3 velocityMax = { position.x + size.x / 2.0f,
								position.y + size.y / 2.0f,
								position.z + size.z / 2.0f };
	Vector3 velocityMin = { position.x - size.x / 2.0f,
								position.y - size.y / 2.0f,
								position.z - size.z / 2.0f };
	std::uniform_real_distribution<float> velocityX(velocityMin.x, velocityMax.x);
	std::uniform_real_distribution<float> velocityY(velocityMin.y, velocityMax.y);
	std::uniform_real_distribution<float> velocityZ(velocityMin.z, velocityMax.z);

	velocity_ = { velocityX(randomEngine), velocityY(randomEngine), velocityZ(randomEngine) };

	const float pram = 1.0f / lifeTime_ * 60.0f;
	acceleration_ = { velocity_.x * pram, velocity_.y * pram, velocity_.z * pram };

}

void SparkParticle::Update(const Matrix4x4& billBoardMatrix)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	velocity_ = vector3Calc->Subtract(velocity_, acceleration_);

	transform_.translate = vector3Calc->Add(transform_.translate, velocity_);

	IParticle::Update(billBoardMatrix);

}
