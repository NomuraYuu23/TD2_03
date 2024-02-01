#pragma once
#include <list>
#include <memory>
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/3D/Model.h"
#include "BlockManagerState/IBlockManagerState.h"
#include "BlockManagerState/BlockManagerFactory.h"
#include "BlockGenerationPattern/BlockGenerationPatternFile.h"

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

// ブロックの速度
enum BlockSpeedIndex {
	kBlockSpeedIndexFast,
	kBlockSpeedIndexMiddle,
	kBlockSpeedIndexSlow,
	kBlockSpeedIndexOfCount,
};

// ブロックのマテリアル
enum BlockMaterialIndex {
	kBlockMaterialIndexWhite, // 白
	kBlockMaterialIndexOfCount,
};

// ブロック生成パターン名
// ここに追加したらBlockManagerのblockGenerationPatternNames_も増やす
enum BlockGenerationPatternName {
	kBlockGenerationPatternNameSample,
	kBlockGenerationPatternNameOfCount,
};

enum BlockGenerationDirection {
	kBlockGenerationDirectionNorth,
	kBlockGenerationDirectionSouth,
	kBlockGenerationDirectionEast,
	kBlockGenerationDirectionWest,
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

private: // ブロック生成

	/// <summary>
	/// ブロック生成
	/// </summary>
	void GenerationBlock(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 中心ブロック生成
	/// </summary>
	void GenerationCenterBlock();

	/// <summary>
	/// 中心ブロック生成
	/// </summary>
	void GenerationCenterBlock(const Vector3& position, const Vector3& size);

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
	/// 方角決定
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	BlockGenerationDirection WhereComeFrom(const Vector3& position);

	Vector3 CreatePosition(BlockGenerationDirection blockGenerationDirection, const Vector3& position);

	Vector3 CreateVelocity(BlockGenerationDirection blockGenerationDirection);

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
	std::list<Block*>& GetBlocks() { return blocks_; }

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

	// ブロック生成のクールタイムカウント(フレーム)
	uint32_t generationBlockFrameCount_;

	// ブロック生成のクールタイム(フレーム)
	uint32_t generationBlockFrame_;

	// ブロック生成数
	uint32_t generatedBlocksNum_;
	// 一回のブロック最大生成数
	uint32_t generatedBlocksNumMax_ = 2;

	// 方角
	BlockGenerationDirection blockGenerationDirection_;

	// 生成幅
	float generatedWidth_ = 75.0f;

	// 速度
	std::array<float, kBlockSpeedIndexOfCount> blockSpped_;

private: // モデルなど描画系

	Model* model_ = nullptr;

	std::array<std::unique_ptr<Material>, kBlockMaterialIndexOfCount> materials_;

};

