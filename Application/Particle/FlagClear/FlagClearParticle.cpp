#include "FlagClearParticle.h"
#include <random>
#include "../../../Engine/Math/Ease.h"

void FlagClearParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	transform_.scale = size;
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = position;

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	lifeTime_ = 0.2f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	billBoardName_ = kBillBoardNameIndexAllAxis;

	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

	startPosition_ = position;

	Vector3 endMax = { 10.0f, 10.0f, 10.0f };
	Vector3 endMin = { -10.0f, -10.0f, -10.0f };
	std::uniform_real_distribution<float> endX(endMin.x, endMax.x);
	std::uniform_real_distribution<float> endY(endMin.y, endMax.y);
	std::uniform_real_distribution<float> endZ(endMin.z, endMax.z);

	endPosition_ = { endX(randomEngine), endY(randomEngine), endZ(randomEngine) };

}

void FlagClearParticle::Update(const Matrix4x4& billBoardMatrix)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	float t = currentTime_ / lifeTime_;

	transform_.translate = Ease::Easing(Ease::EaseName::Lerp, startPosition_, endPosition_, t);

	IParticle::Update(billBoardMatrix);

}
