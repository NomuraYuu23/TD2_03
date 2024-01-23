#include "BlockManagerStateInit.h"
#include "../../BlockManager.h"

void BlockManagerStateInit::Initialize(BlockManager* blockManager)
{

	// ブロックマネージャー
	blockManager_ = blockManager;

	// 中心ブロック生成
	blockManager_->GenerationCenterBlock();

	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,-12.0f}, {2.0f,1.0f,4.0f});
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,-12.0f }, { 2.0f,1.0f,4.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f,-12.0f }, { 2.0f,1.0f,4.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f,-12.0f }, { 2.0f,1.0f,4.0f });

	blockManager_->GenerationCenterBlock({ -50.0f,0.0f,12.0f }, { 2.0f,1.0f,4.0f });
	blockManager_->GenerationCenterBlock({ -100.0f,0.0f,12.0f }, { 2.0f,1.0f,4.0f });
	blockManager_->GenerationCenterBlock({ 50.0f,0.0f, 12.0f }, { 2.0f,1.0f,4.0f });
	blockManager_->GenerationCenterBlock({ 100.0f,0.0f, 12.0f }, { 2.0f,1.0f,4.0f });

	// ブロック
	Vector3 position = { 2.0f, 0.0f, 60.0f };
	Vector3 velocity = { 0.1f, 0.0f, 0.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -200.0f,0.0f,-30.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -230.0f,0.0f,-50.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -250.0f,0.0f,-30.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -300.0f,0.0f,-50.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -320.0f,0.0f,-30.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -340.0f,0.0f,-60.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -400.0f,0.0f,-80.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -420.0f,0.0f,-30.0f };
	blockManager_->GenerationBlock(position, velocity);

	position = { -440.0f,0.0f,-70.0f };
	blockManager_->GenerationBlock(position, velocity);

	blockManagerStateNo_ = kBlockManagerState1;

}

void BlockManagerStateInit::Update()
{
}

void BlockManagerStateInit::GenerationBlock(uint32_t maxGenerationBlock)
{
}
