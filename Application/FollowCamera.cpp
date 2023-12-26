#include "FollowCamera.h"
#include <algorithm>
#include <numbers>
#include "../Engine/Input/Input.h"
void FollowCamera::Initialize()
{
	BaseCamera::Initialize();
}
void FollowCamera::Update()
{
	Input* input_ = Input::GetInstance();
	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();
	if (1)
	{

		const float rotateSpeed = 0.1f;

		transform_.rotate.y += float(input_->GetRightAnalogstick().x) / SHRT_MAX * rotateSpeed;
		transform_.rotate.x -= float(input_->GetRightAnalogstick().y) / SHRT_MAX * rotateSpeed;
		transform_.rotate.x = std::clamp(transform_.rotate.x, float(-std::numbers::pi) / 2.0f + 0.1f, float(std::numbers::pi) / 2.0f - 0.1f);
	}

	Matrix4x4 rotateMatrix1 = calc->MakeRotateXYZMatrix(transform_.rotate);
	rotateMatrix3_ = calc->MakeIdentity4x4();
	if (target_)
	{
		Vector3 offset = { 0.0f,3.0f,-20.0f };


		
		//rotateMatrix_ = rotateMatrix1 * rotateMatrix2_ * rotateMatrix3_;
		rotateMatrix_ = rotateMatrix1;
		offset = calc->TransformNormal(offset, rotateMatrix_);

		//interTargert_ = Lerp(interTargert_, target_->GetWorldPosition(), cameraDelay_);

		//transform_.translate = interTargert_ + offset;
		transform_.translate = Vector3Calc::Add(interTargert_ , offset);
	}

	
	//viewProjection_.UpdateMatrix();
	viewMatrix_ = calc->Inverse(calc->Multiply(rotateMatrix_ , calc->MakeTranslateMatrix(transform_.translate)));
	//viewMatrix_ = calc->Inverse(transformMatrix_);
	projectionMatrix_ = calc->MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_ = calc->Multiply(viewMatrix_, projectionMatrix_);

	worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };

}

void FollowCamera::Reset() {


	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();
	rotateMatrix2_ = calc->MakeIdentity4x4();
	if (target_)
	{
		Vector3 offset = { 0.0f,2.0f,-20.0f };

		Matrix4x4 rotateMatrix = calc->MakeRotateXYZMatrix(transform_.rotate);

		offset = calc->TransformNormal(offset, rotateMatrix);

		transform_.translate = Vector3Calc::Add(interTargert_, offset);
		interTargert_ = target_->GetWorldPosition();
	}

	//viewProjection_.UpdateMatrix();
}