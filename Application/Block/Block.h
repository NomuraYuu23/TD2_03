#pragma once

#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Screw/Screw.h"
#include <array>
class Block {
public:

	struct AnchorPoint
	{
		Vector3 position;//立方体上の位置
		//bool isStings;//刺さってるか
		Screw* screw;//ネジのポインタ
	};

	static const size_t anchorNum = 1;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw(Model* model,BaseCamera& camera);

	inline WorldTransform* GetWorldTransform() { return &worldTransform_; };

	std::array<AnchorPoint, anchorNum>& GetAnchorPointArray() { return anchorPoints_; };
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
	bool GetIsRidePlayer() { return isRidePlayer_; };
	void SetIsRidePlayer(bool is) { isRidePlayer_ = is; };
	void SetSize(const Vector3& vector) { worldTransform_.transform_.scale = vector; };
	void SetWhiteTextureHandle(uint32_t th) { whiteTextureHandle_ = th; };
	void SetReConnect(bool is) { reConnect_ = is; }; //再構成時に演出を出さない用
protected:

	std::array<AnchorPoint, anchorNum> anchorPoints_;
	WorldTransform worldTransform_;
	std::unique_ptr<OBB> collider_;
	bool isConnect_;//中心の足場とくっついている(もしくは中心の足場)かどうか
	bool isCenter_;//中心となる足場か

	Vector3 velocity_;
	bool isRelese_;//ネジが外されたフレームだけtrueになる
	bool isRidePlayer_; //プレイヤーが上に乗っているか
	std::unique_ptr <Material> mat_;
	WorldTransform worldTransformOverRay_;
	uint32_t whiteTextureHandle_;
	bool isConnectAnimation_;
	bool reConnect_ = false;
	float alpha_;

	std::unique_ptr <Material> mat2_;
};