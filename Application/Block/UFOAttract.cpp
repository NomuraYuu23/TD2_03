#include "UFOAttract.h"

void UFOAttract::Initialize() {
	collider_.reset(new Sphere);
	collider_->Initialize({ 0,0,0 }, radius_, this);
	screwCount_ = 0;
	isAttract_ = false;
}

