#pragma once
#include "../../../Engine/Particle/IParticle.h"
class FlagClearParticle : public IParticle
{

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FlagClearParticle() override {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	void Initialize(const Vector3& position, const Vector3& size) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="billBoardMatrix"></param>
	void Update(const Matrix4x4& billBoardMatrix) override;

private:

	// 初期位置
	Vector3 startPosition_;

	// 終了位置
	Vector3 endPosition_;

};

