#include "GravityParticle.h"
#include "../../Engine/Math/RandomEngine.h"
#include "../../Engine/Math/Ease.h"
void GravityParticle::Initialize(const Vector3& position, const Vector3& size) {
	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	
	transform_.scale = { 0.5f,0.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = position;

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = { 1.0f,1.0f, 1.0f ,1.0f};

	lifeTime_ = 0.5f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

	endPosition_ = position;
	startPosition_=position;
	float y = RandomEngine::GetRandom(1.0f, 6.0f);
	startPosition_.y+= y;
}

void GravityParticle::Update(const Matrix4x4& billBoardMatrix) {
	if (lifeTime_ <= currentTime_) {
		isDead_ = true;
	}
	float t = currentTime_ / lifeTime_;
	transform_.translate = Ease::Easing(Ease::EaseName::Lerp,startPosition_,endPosition_,t);
	float slidet = (t - 0.5f)*2.0f;
	float st = ((- slidet * slidet) + 1.0f)/2.0f;
	float length = std::sqrtf((startPosition_.y-endPosition_.y) * (startPosition_.y - endPosition_.y));
	transform_.scale.y = st * length;
	//transform_.scale.y = 1.0f;
	if (startPosition_.y < endPosition_.y) {
		transform_.scale.y = 1.0f;
	}

	TimeElapsed();
	UpdateMatrix(billBoardMatrix);
}