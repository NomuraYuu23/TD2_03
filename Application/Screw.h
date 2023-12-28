#pragma once
#include "../Engine/3D/WorldTransform.h"
#include "../Engine/3D/Model.h"
#include "../Engine/Camera/BaseCamera.h"
#include "../Engine/Math/Vector3.h"

class Player;
class Block;

class Screw
{
public:

	enum State {
		NONE,
		FOLLOW,
		REVERSE,
		TOBLOCK,
		STUCK,
		TOPLAYER,
	};

	void Initialize();
	void Update();
	void Draw(Model* model, BaseCamera& camera);

	void Throw(const Vector3 position, void* block , size_t num);
	void Catch();

	void None() {};
	void Follow();
	void Reverse() {};
	void ToBlock();
	void Stuck() {};
	void ToPlayer();

	State GetState() { return state_; };
	void SetPlayer(Player* p) { player_ = p; };
private:
	WorldTransform worldTransform_;
	Vector3 startPosition_; //補間アニメーションの開始位置
	void* target_;
	size_t targetNum_;
	int frameCount_;

	State state_;
	static void(Screw::* stateTable[])();
	Player* player_;
};
