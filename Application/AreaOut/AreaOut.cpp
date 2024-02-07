#include "AreaOut.h"

void AreaOut::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	isInvisible_ = true;

	// ワールド変換データの初期化
	worldTransform_.Initialize();

}

void AreaOut::Update(const Vector3& playerPos)
{

	if (std::fabsf(playerPos.x) > invisibleLimit_ ||
		std::fabsf(playerPos.z) > invisibleLimit_) {
		isInvisible_ = false;
	}
	else {
		isInvisible_ = true;
		alpha_ = 3.14f;
	}

	Flickering();

	UvMove();

	worldTransform_.UpdateMatrix();

}

void AreaOut::Draw(BaseCamera& camera)
{

	if (isInvisible_) {
		return;
	}

	model_->Draw(worldTransform_, camera, material_.get());

}

void AreaOut::ImGuiDraw()
{
}

void AreaOut::Flickering()
{
	//α
	alpha_ = std::fmodf(alpha_ + 0.05f, 6.28f);

	material_->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, std::fabsf(std::sinf(alpha_)) });

}

void AreaOut::UvMove()
{

	move_ = std::fmodf(move_ + 0.002f, 1.0f);

	TransformStructure transformStructure;
	transformStructure.scale = { 1.0f,1.0f,1.0f };
	transformStructure.rotate = { 0.0f,0.0f,0.0f };
	transformStructure.translate = { move_,0.0f,0.0f };

	material_->SetUvTransform(transformStructure);

}
