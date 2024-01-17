#include "BlockManagerStateInit.h"
#include "../../BlockManager.h"

void BlockManagerStateInit::Initialize(BlockManager* blockManager)
{

	// ブロックマネージャー
	blockManager_ = blockManager;

	// 中心ブロック生成
	blockManager_->GenerationCenterBlock();

	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,-12.0f}, {6.0f,0.5f,12.0f});
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f,-12.0f }, { 6.0f,0.5f,12.0f });

	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f, 12.0f }, { 6.0f,0.5f,12.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f, 12.0f }, { 6.0f,0.5f,12.0f });

	// ブロック
	Vector3 position = { 2.0f, 0.0f, 60.0f };
	Vector3 velocity = { 0.0f, 0.0f, -0.1f };
	blockManager_->GenerationBlock(position, velocity);

	position = { 2.0f,0.0f,90.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { 60.0f,0.0f,20.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -10.0f,0.0f,150.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -30.0f,0.0f,200.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { 2.0f,0.0f,250.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { 2.0f,0.0f,300.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -20.0f,0.0f,310.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -10.0f,0.0f,350.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -30.0f,0.0f,450.0f };
	blockManager_->GenerationBlock(position, velocity);

	blockManagerStateNo_ = kBlockManagerState1;

}

void BlockManagerStateInit::Update()
{
}

void BlockManagerStateInit::GenerationBlock(uint32_t maxGenerationBlock)
{
}
