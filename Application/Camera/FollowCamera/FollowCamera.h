#pragma once
//#include "../../../Engine/3D/ViewProjection.h"
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"

class FollowCamera : public BaseCamera{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新＋カメラが引く
	/// </summary>
	/// <param name="connectCount"></param>
	void Update(int connectCount);

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	void SetDestinationAngle(const Vector3& destinationAngle) { destinationAngle_ = destinationAngle; }
	void Shake();
	int32_t GetControlLength() { return controlLength_; };
private: // メンバ関数

	// 追従対象からのオフセットを計算する
	Vector3 OffsetCalc() const;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:

	//追従対象
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	Vector3 destinationAngle_ = { 0.0f,0.0f,0.0f };

	float moveRate_ = 0.1f;
	float rotateRate_ = 0.1f;

	float offsetLength_ = -100.0f;

	bool isShake_;
	float shakeSize_;

	// オフセットの長さ 最小最大 作りの形的にマイナスで
	float offsetLengthMin_ = -100.0f;
	float offsetLengthMax_ = -200.0f;
	 // オフセットの長さ 媒介変数
	float offsetLengthT_ = 0.0f;
	// 長さを決めるブロックの数
	float offsetLengthConnectMax_ = 10.0f;
	// オフセットの長さ 媒介変数速度
	float offsetLengthTSpeed_ = 0.05f;
	//視点移動を押してる長さ(累計)
	int32_t controlLength_;

};

