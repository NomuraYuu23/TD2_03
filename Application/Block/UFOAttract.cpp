#include "UFOAttract.h"
#include "../Screw/Screw.h"
void UFOAttract::Initialize() {
	collider_.reset(new Sphere);
	collider_->Initialize({ 0,0,0 }, radius_, this);
	screwCount_ = 0;
	isAttract_ = false;
}

void UFOAttract::OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {
	if (std::holds_alternative<Screw*>(pairObject)&& std::get<Screw*>(pairObject)->GetState() != Screw::STUCK) {
		screwCount_++;
	}
};