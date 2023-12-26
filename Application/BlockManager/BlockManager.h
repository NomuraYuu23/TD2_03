#pragma once
#include <list>
#include <memory>
#include "../../Engine/Math/Vector3.h"
#include "BlockManagerState/IBlockManagerState.h"
#include "BlockManagerState/BlockManagerFactory.h"

// ブロック
class Block;

enum BlockManagerState {
	kBlockManagerStateOfCount // 数
};

/// <summary>
/// ブロックマネージャー
/// </summary>
class BlockManager
{

public:

	~BlockManager();

	void Initialize();

	void Update();

	void Draw();

	void ImGuiDraw();

private: // メンバ関数

	// ブロックの範囲制御
	void RangeControl();

	// ブロックの数制御
	uint32_t NumberControl();

	// 状態切り替え
	void StateChange();

private: // メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	// ブロックの最大位置
	Vector3 maxBlockPosition_;

	// ブロックの最大数
	uint32_t maxBlock_;

	// ステート(ブロックの生成パターン)
	std::unique_ptr<IBlockManagerState> blockManagerState_;

	// ステート切り替え(ファクトリー)
	BlockManagerFactory* blockManagerFactory_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

};

