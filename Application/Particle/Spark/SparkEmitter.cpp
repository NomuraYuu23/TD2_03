#include "SparkEmitter.h"
#include "../MakeParticle.h"

void SparkEmitter::Initialize(const TransformStructure& transform, uint32_t instanceCount, float frequency, float lifeTime, uint32_t particleModelNum, uint32_t paeticleName)
{
	IEmitter::Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
}

void SparkEmitter::Update()
{
	IEmitter::Update();
}

std::list<IParticle*> SparkEmitter::Emit()
{

	std::list<IParticle*> particles;
	MakeParticle* makeParticle = MakeParticle::GetInstance();

	for (uint32_t count = 0; count < instanceCount_; ++count) {
		particles.push_back(makeParticle->Run(paeticleName_, transform_.translate, transform_.scale));
	}

	return particles;

}
