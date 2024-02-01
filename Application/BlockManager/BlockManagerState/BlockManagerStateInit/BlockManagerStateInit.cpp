#include "BlockManagerStateInit.h"
#include "../../BlockManager.h"

void BlockManagerStateInit::Initialize(BlockManager* blockManager)
{

	// ブロックマネージャー
	blockManager_ = blockManager;

	// 中心ブロック生成
	//blockManager_->GenerationCenterBlock();
	/*
	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,-12.0f}, {6.0f,0.5f,12.0f});
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });

	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f, 12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f, 12.0f }, { 6.0f,0.5f,12.0f });
	*/

	blockManagerStateNo_ = kBlockManagerState1;

}

void BlockManagerStateInit::Update()
{
}

void BlockManagerStateInit::GenerationBlock(uint32_t maxGenerationBlock)
{
}
