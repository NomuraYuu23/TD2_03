#pragma once
#include "Block.h"
#include "UFOAttract.h"
#include "../Magnet.h"
class UFO : public Block
{
public:
	void Initialize() override;
	void Update() override;
	void Draw(Model* model, BaseCamera& camera) override;

	void SetCircle(Model* m) { modelCircle_ = m; };

	OBB* GetCollider() { return colliderUFO_.get(); };
	Magnet* GetMagnet() { return magnet_.get(); };
	UFOAttract* GetAttract() { return attract_.get(); };

	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData);
	bool GetIsDead() { return isDead_; };
private:
	bool isDead_;
	std::unique_ptr<OBB> colliderUFO_;
	std::unique_ptr<Magnet> magnet_;
	std::unique_ptr<UFOAttract> attract_;
	//std::unique_ptr<OBB> collider_;
	Model* modelCircle_;
	std::unique_ptr <Material> materialCircle_;
	WorldTransform worldTransformCircle_;
	float theta_;
	int screwCount_;//範囲内にネジが何体いるか
};
