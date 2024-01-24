#pragma once
#include "../../../Engine/Particle/IEmitter.h"
class SparkEmitter : public IEmitter
{

public:
	~SparkEmitter() override {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const TransformStructure& transform, uint32_t instanceCount,
		float frequency, float lifeTime, uint32_t particleModelNum, uint32_t paeticleName) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	std::list<IParticle*> Emit() override;

private:
	float radius_;

};

