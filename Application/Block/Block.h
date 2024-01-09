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

	virtual void Initialize();
	virtual void Update();
	virtual void Draw(Model* model,BaseCamera& camera);

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	std::array<AnchorPoint, 4>& GetAnchorPointArray() { return anchorPoints_; };
	Vector3 GetAnchorPointWorldPosition(size_t num);
	Screw* GetAnchorPointScrew(size_t num) { return anchorPoints_[num].screw; };
	Screw* SetAnchorPointScrew(size_t num,Screw* screw) { return anchorPoints_[num].screw = screw; };
	OBB* GetCollider() { return collider_.get(); };
	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData);
	bool GetIsConnect() { return isConnect_; };
	void SetIsConnect(bool is) { isConnect_ = is; };
	bool GetIsCenter() { return isCenter_; };
	void SetIsCenter(bool is) { isCenter_ = is; };
	void SetWorldPosition(const Vector3& pos) { worldTransform_.transform_.translate = pos; };
	void SetVelocity(const Vector3& v) { velocity_ = v; };
	bool GetIsRelese() { return isRelese_; };
protected:

	std::array<AnchorPoint,4> anchorPoints_;
	WorldTransform worldTransform_;
	std::unique_ptr<OBB> collider_;
	bool isConnect_;//中心の足場とくっついている(もしくは中心の足場)かどうか
	bool isCenter_;//中心となる足場か

	Vector3 velocity_;
	bool isRelese_;//ネジが外されたフレームだけtrueになる
};