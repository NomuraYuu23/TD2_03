#include "LinerEmitter.h"
#include "ForLinerEmitterData.h"
#include "../MakeParticle.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/RandomEngine.h"
void LinerEmitter::Initialize(const TransformStructure& transform, uint32_t instanceCount,
	float frequency, float lifeTime, uint32_t particleModelNum, uint32_t paeticleName) {
	IEmitter::Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	
}

void LinerEmitter::Update() {
	//IEmitter::Update();
	toEmit_ = false;
	if (ForLinerEmitterData::GetInstance()->GetIsDraw()) {
		toEmit_ = true;
	}
	
}

std::list<IParticle*> LinerEmitter::Emit()
{

	std::list<IParticle*> particles;
	MakeParticle* makeParticle = MakeParticle::GetInstance();

	Vector3 translate;
	Vector3 start = ForLinerEmitterData::GetInstance()->GetStart();
	Vector3 end = ForLinerEmitterData::GetInstance()->GetEnd();
	for (uint32_t count = 0; count < instanceCount_; ++count) {
		translate = Vector3Calc::Lerp(start,end,float(count)/float(instanceCount_-1));
		particles.push_back(makeParticle->Run(paeticleName_, translate, transform_.scale));
	}

	return particles;
}