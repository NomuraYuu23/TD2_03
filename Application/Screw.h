#pragma once
#include "../Engine/3D/WorldTransform.h"
#include "../Engine/3D/Model.h"
#include "../Engine/Camera/BaseCamera.h"
#include "../Engine/Math/Vector3.h"
#include <array>

class Screw
{
public:

	enum State {
		NONE,
		FOLLOW,
		TOBLOCK,
		STUCK,

	};

	void Initialize();
	void Update();
	void Draw(Model* model, BaseCamera& camera);
private:
	WorldTransform worldTransform_;

};
