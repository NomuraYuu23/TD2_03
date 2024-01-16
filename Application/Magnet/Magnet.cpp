#include "Magnet.h"

void Magnet::Initialize() {
	collider_.reset(new Sphere);
	collider_->Initialize({0,0,0},radius_, this);
}

