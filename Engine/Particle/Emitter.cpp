#include "Emitter.h"
#include "../Math/DeltaTime.h"
#include <cassert>

void Emitter::Initialize(const TransformStructure& transform, uint32_t instanceCount,
	float frequency, float lifeTime, uint32_t particleModelNum, uint32_t paeticleName)
{

	// トランスフォーム
	transform_ = transform;
	// 発生させるインスタンス数
	instanceCount_ = instanceCount;
	// 発生頻度
	frequency_ = frequency;
	// 発生時間
	frequencyTime_ = 0.0f;
	// 生存時間
	lifeTime_ = lifeTime;
	// 発生してからの経過時間
	currentTime_ = 0.0f;
	// エミットする
	toEmit_ = false;

	isDead_ = false;

	particleModelNum_ = particleModelNum;
	
	paeticleName_ = static_cast<PaeticleName>(paeticleName);

}

void Emitter::Update()
{

	frequencyTime_ += kDeltaTime_;
	if (frequencyTime_ >= frequency_) {
		frequencyTime_ = frequencyTime_ - frequency_;
		toEmit_ = true;
	}
	currentTime_ += kDeltaTime_;
	if (currentTime_ >= lifeTime_) {
		isDead_ = true;
	}

}

std::list<Particle*> Emitter::Emit()
{

	std::list<Particle*> particles;
	for (uint32_t count = 0; count < instanceCount_; ++count) {
		particles.push_back(MakeParticle());
	}

	return particles;
}

Particle* Emitter::MakeParticle()
{

	Particle* particle = nullptr;

	switch (paeticleName_)
	{
	case kDefault:
		particle = new Particle();
		particle->Initialize(transform_.translate, transform_.scale);
		break;
	case kCountOfPaeticleName:
	default:
		assert(0);
		break;
	}

	return particle;

}
