#pragma once
#include "../IBlockManagerState.h"
class BlockManagerStatePattern1 : public IBlockManagerState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ブロックの生成
	/// </summary>
	/// <param name="maxGenerationBlock">生成できる最大数</param>
	void GenerationBlock(uint32_t maxGenerationBlock) override;

private: 

	// 生成ブロック数

	// 


};

