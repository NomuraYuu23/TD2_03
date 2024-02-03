#include "MakeEmitter.h"
#include <cassert>
#include "EmitterName.h"
#include "Gravity/GravityEmitter.h"	
#include "Spark/SparkEmitter.h"
#include "Liner/LinerEmitter.h"
#include "FlagClear/FlagClearEmitter.h"
MakeEmitter* MakeEmitter::GetInstance()
{
    return nullptr;
}

IEmitter* MakeEmitter::Run(const TransformStructure& transform, uint32_t instanceCount,
	float frequency, float lifeTime,
	uint32_t particleModelNum, uint32_t paeticleName, uint32_t emitterName)
{
	
	IEmitter* emitter = nullptr;

	switch (emitterName)
	{
	case kDefaultEmitter:
		emitter = new IEmitter();
		emitter->Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
		break;
	case kGravityEmitter:
		emitter = new GravityEmitter();
		emitter->Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
		break;
	case kSparkEmitter:
		emitter = new SparkEmitter();
		emitter->Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
		break;
	case kLinerEmitter:
		emitter = new LinerEmitter();
		emitter->Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
		break;
	case kFlagClearEmitter:
		emitter = new FlagClearEmitter();
		emitter->Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
		break;
	case kCountOfParticleName:
	default:
		assert(0);
		break;
	}

	return emitter;

}
