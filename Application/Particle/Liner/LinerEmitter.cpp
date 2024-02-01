#include "LinerEmitter.h"
#include "../MakeParticle.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/RandomEngine.h"
void LinerEmitter::Initialize(const TransformStructure& transform, uint32_t instanceCount,
	float frequency, float lifeTime, uint32_t particleModelNum, uint32_t paeticleName) {
	IEmitter::Initialize(transform, instanceCount, frequency, lifeTime, particleModelNum, paeticleName);
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Player";
	radius_ = globalVariables->GetFloatValue(groupName, "MagnetRadius");
}

void LinerEmitter::Update() {
	IEmitter::Update();
}

std::list<IParticle*> LinerEmitter::Emit()
{

	std::list<IParticle*> particles;
	MakeParticle* makeParticle = MakeParticle::GetInstance();

	Vector3 translate = transform_.translate;
	translate.x = RandomEngine::GetRandom(-1.0f, 1.0f);
	translate.z = RandomEngine::GetRandom(-1.0f, 1.0f);
	float length = std::sqrtf(std::powf(1.0f, 2) + std::powf(1.0f, 2));
	if (length != 0.0f) {
		translate.x /= length;
		translate.z /= length;
	}
	translate.x *= radius_;
	translate.z *= radius_;
	translate.x += transform_.translate.x;
	translate.z += transform_.translate.z;
	for (uint32_t count = 0; count < instanceCount_; ++count) {
		particles.push_back(makeParticle->Run(paeticleName_, translate, transform_.scale));
	}

	return particles;
}