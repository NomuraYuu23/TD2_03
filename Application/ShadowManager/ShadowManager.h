#pragma once
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/3D/Model.h"
class ShadowManager
{

public:

	struct ShadowObj {
		Vector3 position_;
		Vector3 size_;
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// リストのクリア
	/// </summary>
	void ListClear();

	/// <summary>
	/// 影を発生させるオブジェクトリスト登録
	/// </summary>
	/// <param name="position"></param>
	void CastsShadowObjListRegister(const Vector3& position, const Vector3& size);

	/// <summary>
	/// 影が現れるオブジェクトリスト登録
	/// </summary>
	/// <param name="position"></param>
	void ShadowAppearsObjListRegister(const Vector3& position, const Vector3& size);

	/// <summary>
	/// 影が出るか
	/// </summary>
	void SeeShadow();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BaseCamera& baseCamera);

private: // 関数

	/// <summary>
	/// Yから見て重なるか
	/// </summary>
	bool OverlapY(const ShadowObj& a, const ShadowObj& b);

private: // 定数

	// 影の数
	static const uint32_t kShadowMax_ = 128;
	
private:

	// 影を発生させるオブジェクトリスト
	std::list<ShadowObj> castsShadowObjList_;

	// 影が現れるオブジェクトリスト
	std::list<ShadowObj> shadowAppearsObjList_;

	// 影のモデル
	Model* model_;

	// ワールドトランスフォーム
	std::array<WorldTransform, kShadowMax_> worldTransforms_;

	// 影を出す数
	uint32_t shadowCount_;

};

