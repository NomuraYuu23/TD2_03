#pragma once
#include <cstdint>

class BlockManager;

enum BlockManagerState {
	kBlockManagerStateInit, // 初期
	kBlockManagerState1, // 1
	kBlockManagerStateOfCount // 数
};

class IBlockManagerState
{

protected:

	// ブロックマネージャーの状態番号
	static uint32_t blockManagerStateNo_;

	// ブロックマネージャー
	static BlockManager* blockManager_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ブロックの生成
	/// </summary>
	/// <param name="maxGenerationBlock">生成できる最大数</param>
	virtual void GenerationBlock(uint32_t maxGenerationBlock) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IBlockManagerState() = default;

	/// <summary>
	/// ブロックマネージャーセット
	/// </summary>
	/// <param name="blockManager"></param>
	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	/// <summary>
	/// ブロックマネージャーの状態番号取得
	/// </summary>
	/// <returns>ブロックマネージャーの状態番号</returns>
	uint32_t GetBlockManagerStateNo() { return blockManagerStateNo_; }

};

