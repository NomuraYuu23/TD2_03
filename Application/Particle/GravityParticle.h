#pragma once
#include "../../Engine/Particle/IParticle.h"

class GravityParticle : public IParticle
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GravityParticle() override {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="numInstance"></param>
	void Initialize(const Vector3& position, const Vector3& size) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cameraMatrix4x4"></param>
	void Update(const Matrix4x4& billBoardMatrix) override;
private:
	Vector3 startPosition_;
	Vector3 endPosition_;
	Vector3 colorRGB_ = {0.4f,0.1f,0.4f};
};

