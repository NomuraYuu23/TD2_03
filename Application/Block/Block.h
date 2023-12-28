#pragma once

#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Screw.h"
#include <array>
class Block {
public:

	struct AnchorPoint
	{
		Vector3 position;//立方体上の位置
		//bool isStings;//刺さってるか
		Screw* screw;//ネジのポインタ
	};

	void Initialize();
	void Update();
	void Draw(Model* model,BaseCamera& camera);

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	std::array<AnchorPoint, 4>& GetAnchorPointArray() { return anchorPoints_; };
	Vector3 GetAnchorPointWorldPosition(size_t num);
	Screw* GetAnchorPointScrew(size_t num) { return anchorPoints_[num].screw; };
	Screw* SetAnchorPointScrew(size_t num,Screw* screw) { return anchorPoints_[num].screw = screw; };
	OBB* GetCollider() { return collider_.get(); };
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData) {};
private:

	std::array<AnchorPoint,4> anchorPoints_;
	WorldTransform worldTransform_;
	std::unique_ptr<OBB> collider_;
};