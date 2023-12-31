#pragma once
#include <list>
#include <memory>
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/3D/Model.h"
#include "BlockManagerState/IBlockManagerState.h"
#include "BlockManagerState/BlockManagerFactory.h"

// ブロック
class Block;

// プレイヤー
class Player;

// ブロックの大きさ
enum BlockSizeIndex {
	kBlockSizeIndexSmall,
	kBlockSizeIndexMiddle,
	kBlockSizeIndexBig,
	kBlockSizeIndexOfCount,
};

// ブロックのマテリアル
enum BlockMaterialIndex {
	kBlockMaterialIndexWhite, // 白
	kBlockMaterialIndexOfCount,
};

/// <summary>
/// ブロックマネージャー
/// </summary>
class BlockManager
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">ブロックのモデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ブロックの描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // メンバ関数

	/// <summary>
	/// ブロックの範囲制御
	/// </summary>
	void RangeControl();

	/// <summary>
	/// ブロックの数制御
	/// </summary>
	/// <returns>生成しても大丈夫な数</returns>
	uint32_t NumberControl();

	/// <summary>
	///	状態切り替え
	/// </summary>
	void StateChange();

	/// <summary>
	/// ブロック生成
	/// </summary>
	void GenerationBlock();

	/// <summary>
	/// マテリアル初期化
	/// </summary>
	void MaterialInitialize();

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

	/// <summary>
	/// プレイヤーセット
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ブロックゲット
	/// </summary>
	/// <returns>ブロック</returns>
	std::list<Block*> GetBlocks() { return blocks_; }

private: // メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	// ブロックの最大位置
	Vector3 maxBlockPosition_;

	// ブロックの最小位置
	Vector3 minBlockPosition_;

	// ブロックの最大数
	uint32_t maxBlock_;

	// ブロックのサイズ
	std::array<Vector3, kBlockSizeIndexOfCount> blockSize_;

	// プレイヤー
	Player* player_;

private: // ステート

	// ステート(ブロックの生成パターン)
	std::unique_ptr<IBlockManagerState> blockManagerState_;

	// ステート切り替え(ファクトリー)
	BlockManagerFactory* blockManagerFactory_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

private: // モデルなど描画系

	Model* model_ = nullptr;

	std::array<std::unique_ptr<Material>, kBlockMaterialIndexOfCount> materials_;

};

