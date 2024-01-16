#pragma once
#include "../Block/UFO.h"
#include "../../Engine/3D/Model.h"
class UFOManager
{
public:
	void Initialize();
	void Update();
	void SetUFOVector(std::vector<std::unique_ptr<UFO>>* list) { ufos_ = list; };
	void SetModelCircle(Model* m) { modelCircle_ = m; };
private:
	std::vector<std::unique_ptr<UFO>>* ufos_;
	int32_t timer_;//秒刻みのカウント
	int32_t frameCount_;
	int32_t spawnInterval_ = 180;
	Model* modelCircle_;
};
