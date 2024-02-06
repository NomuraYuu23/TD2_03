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
	Vector3 toVector = Vector3Calc::Normalize(Vector3Calc::Subtract(end, start));
	float addLength = 2.0f;
	if (!(start.x == end.x && start.z == end.z)) {
		float length = Vector3Calc::Length(Vector3Calc::Subtract(end, start));
		translate = toVector;
		float length2 = Vector3Calc::Length(toVector);
		while (length-2.0f > length2) {
			translate = Vector3Calc::Multiply(addLength,toVector);
			length2 = Vector3Calc::Length(translate);
			translate = Vector3Calc::Add(translate,start);
			particles.push_back(makeParticle->Run(paeticleName_, translate, transform_.scale));
			addLength+=2.0f;
		}
	/*	for (uint32_t count = 0; count < instanceCount_; ++count) {
			translate = Vector3Calc::Lerp(start, end, float(count) / float(instanceCount_ - 1));
			particles.push_back(makeParticle->Run(paeticleName_, translate, transform_.scale));
		}*/
	}

	return particles;
}