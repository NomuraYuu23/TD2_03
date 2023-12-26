#pragma once
#include "IBlockManagerState.h"

class BlockManagerFactory
{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static BlockManagerFactory* GetInstance();

public: // メンバ関数

	// ステート生成
	IBlockManagerState* CreateBlockManagerState(uint32_t BlockManagerStateName);

private:
	BlockManagerFactory() = default;
	~BlockManagerFactory() = default;
	BlockManagerFactory(const BlockManagerFactory&) = delete;
	const BlockManagerFactory& operator=(const BlockManagerFactory&) = delete;

};

