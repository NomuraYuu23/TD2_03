#pragma once
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../../Engine/3D/OutLineData.h"
class Player;
class Block;

class Screw
{
public:

	enum State {
		NONE,
		FOLLOW,
		REVERSE,
		CATCHONPLAYER,
		TOBLOCK,
		STUCK,
		TOPLAYER,
	};

	void Initialize();
	void Update();
	void Draw(Model* model, BaseCamera& camera);
	void DrawOutLine(Model* model, BaseCamera& camera, OutLineData& outLineData);
	void DrawSweat(Model* model, BaseCamera& camera, Matrix4x4& billboard);
	void Throw(const Vector3 position, void* block , size_t num);
	void Catch(WorldTransform* magnetWorldPosition);
	void TurnOver();//ひっくり返す

	void None();
	void Follow();
	void Reverse();
	void CatchOnPlayer();//プレイヤーに掴まれてる状態
	void ToBlock();
	void Stuck();
	void ToPlayer();

	State GetState() { return state_; };
	void SetPlayer(Player* p) { player_ = p; };
	WorldTransform* GetWorldTransform() { return &worldTransform_; };

	void OnCollision(ColliderParentObject pairObject, CollisionData collidionData);
	OBB* GetCollider() { return collider_.get(); };
	void SetReverseTime(int t) { kReverseTime = t; };
	void SetIsDead(bool is) { isDead_ = is; };
	bool GetIsDead() { return isDead_; };
	void SetSweatTextureHandle(uint32_t handle) { sweatTextureHandle_ = handle; };
	bool GetIsDrawSweat() { return isDrawSweat_; };
private:
	WorldTransform worldTransform_;
	Vector3 startPosition_; //補間アニメーションの開始位置
	void* target_;
	size_t targetNum_;
	int frameCount_;

	State state_;
	static void(Screw::* stateTable[])();
	Player* player_;

	std::unique_ptr <Material> mat_;

	float reverseT_;
	int kReverseSpeed_ = 8;//反転の早さ(仮)
	float kFollowSpeed = 0.2f;//追従速度(仮)

	std::unique_ptr<OBB> collider_;

	int kReverseTime = 300;//反転してから戻るまでの長さ

	bool isDead_;

	bool isAttract_;//ufoに吸引されているか

	int32_t kStuckMax = 600;
	int32_t stuckTime_=0;//刺さっている残り時間
	bool isRideBlock_;
	Vector3 followPosition_;
	float followSpeed_;
	Vector3 acceleration_;
	Vector3 velocity_;
	float stuckTop_ = 2.0f;
	float stuckLow_ = -2.0f;

	std::unique_ptr <Material> matSweat_;
	WorldTransform worldTransformSweat_;
	TransformStructure transformSweatUV_;
	uint32_t sweatTextureHandle_;
	bool isDrawSweat_;
	int32_t sweatAnimationChange_ = 10;//遷移時間
	int32_t sweatAnimationframe_;
	int32_t sweatAnimationNum_;//連番の番号
	static const int32_t sweatAnimationMax_ = 3;//アニメーションの枚数
};
