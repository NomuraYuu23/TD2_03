#include "LinerParticle.h"
#include "../../../Engine/Math/RandomEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
void LinerParticle::Initialize(const Vector3& position, const Vector3& size) {
	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	
	colorRGB_ = {1.0f,1.0f,1.0f};
	transform_.scale = { 0.5f,0.5f,0.5f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = position;

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = { 0.8f,0.3f, 0.8f ,1.0f };
	color_.x = colorRGB_.x;
	color_.y = colorRGB_.y;
	color_.z = colorRGB_.z;
	lifeTime_ = 0.3f;
	lifeFrame_ = 1;
	currentTime_ = 0.0f;

	useBillBoard_ = true;
	billBoardName_ = kBillBoardNameIndexAllAxis;
	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

}

void LinerParticle::Update(const Matrix4x4& billBoardMatrix) {
	if (lifeFrame_ <= 0) {
		isDead_ = true;
	}
	lifeFrame_ = 0;
	
	UpdateMatrix(billBoardMatrix);
}