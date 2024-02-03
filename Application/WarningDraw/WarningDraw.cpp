#include "WarningDraw.h"

void WarningDraw::Initialize(const std::array<uint32_t, WarningDrawTextureHandleIndex::kWarningDrawTextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

}

void WarningDraw::Update(const Matrix4x4& viewProjection, std::vector<Screw*> screws)
{

	UIs_[kWarningDrawUIIndexLeftWarning]->SetIsInvisible(true);

	UIs_[kWarningDrawUIIndexRightWarning]->SetIsInvisible(true);

	//α
	alpha = std::fmodf(alpha + 0.05f, 6.28f);
	UIs_[kWarningDrawUIIndexLeftWarning]->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, std::fabsf(std::sinf(alpha)) });
	UIs_[kWarningDrawUIIndexRightWarning]->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, std::fabsf(std::sinf(alpha)) });

	if (screws.empty()) {
		return;
	}

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	for (size_t i = 0; i < screws.size(); i++) {
		Vector3 pos = m4Calc->Transform(screws[i]->GetWorldTransform()->GetWorldPosition(), viewProjection);
		if (pos.x < -1.0f ||
			(((pos.y < -1.0f) || (pos.y > 1.0f)) && pos.x < 0.0f)) {
			UIs_[kWarningDrawUIIndexLeftWarning]->SetIsInvisible(false);
		}
		else if(pos.x >= 1.0f ||
			(((pos.y < -1.0f) || (pos.y > 1.0f)) && pos.x > 0.0f)){
			UIs_[kWarningDrawUIIndexRightWarning]->SetIsInvisible(false);
		}
		if (!UIs_[kWarningDrawUIIndexLeftWarning]->GetIsInvisible() && !UIs_[kWarningDrawUIIndexRightWarning]->GetIsInvisible()) {
			return;
		}

	}

}

void WarningDraw::Draw()
{

	for (size_t i = 0; i < UIs_.size(); i++) {
		UIs_[i]->Draw();
	}

}

void WarningDraw::UIInitialize()
{

	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector2 size = { 512.0f , 512.0f };
	size.x *= 0.15f;
	size.y *= 0.15f;

	position = { 0.0f + size.x, 360.0f };
	UIs_[kWarningDrawUIIndexLeftWarning].reset(
		Sprite::Create(textureHandles_[WarningDrawTextureHandleIndex::kWarningDrawTextureHandleIndexWarning],
			position, color));

	UIs_[kWarningDrawUIIndexLeftWarning]->SetSize(size);

	position = { 1280.0f - size.x, 360.0f };
	UIs_[kWarningDrawUIIndexRightWarning].reset(
		Sprite::Create(textureHandles_[WarningDrawTextureHandleIndex::kWarningDrawTextureHandleIndexWarning],
			position, color));

	UIs_[kWarningDrawUIIndexRightWarning]->SetSize(size);

}
