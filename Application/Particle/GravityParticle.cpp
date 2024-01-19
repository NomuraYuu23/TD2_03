#include "GravityParticle.h"
#include "../../Engine/Math/RandomEngine.h"
#include "../../Engine/Math/Ease.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
void GravityParticle::Initialize(const Vector3& position, const Vector3& size) {
	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "GravityParticle";
	globalVariables->AddItem(groupName, "RGB", colorRGB_);
	colorRGB_ = globalVariables->GetVector3Value(groupName, "RGB");
	transform_.scale = { 0.5f,0.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = position;

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = { 0.8f,0.3f, 0.8f ,1.0f};
	color_.x = colorRGB_.x;
	color_.y = colorRGB_.y;
	color_.z = colorRGB_.z;
	lifeTime_ = 0.3f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;
	billBoardName_ = kBillBoardNameIndexYAxes;
	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

	endPosition_ = position;
	startPosition_=position;
	float y = RandomEngine::GetRandom(2.0f, 15.0f);
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