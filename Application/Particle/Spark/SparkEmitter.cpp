#include "SparkEmitter.h"

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
	return std::list<IParticle*>();
}
