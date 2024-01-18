#pragma once
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collider/OBB/OBB.h"
class Energy
{
public:
	void Initialize();
	void Update();
	void Draw(Model* model, BaseCamera& camera);

	void SetTarget(const Vector3& position);
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData);
	OBB* GetCollider() { return collider_.get(); };
	int32_t GetInnerAreaCount() { return innerAreaCount_; };
private:
	WorldTransform worldTransform_;
	Vector3 target_;
	Vector3 startPoint_;
	std::unique_ptr<OBB> collider_;
	std::unique_ptr <Material> mat_;
	int32_t innerAreaCount_;

	static const size_t kTargetNum = 4;
	std::array<Vector3, kTargetNum> targetList_;
	size_t targetNum_;//何番目の位置に出すか
	int32_t targetChangeLength_=30;//何秒で次の位置に移動するか
	int32_t currentTime_;
	int32_t frameCount_;
};

