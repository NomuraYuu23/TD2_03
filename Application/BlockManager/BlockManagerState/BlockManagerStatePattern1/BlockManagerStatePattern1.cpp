#include "BlockManagerStatePattern1.h"
#include "../../BlockManager.h"

void BlockManagerStatePattern1::Initialize()
{
}

void BlockManagerStatePattern1::Update()
{
}

void BlockManagerStatePattern1::GenerationBlock(uint32_t maxGenerationBlock)
{

	blockManager_->GenerationBlocks(BlockGenerationPatternName::kBlockGenerationPatternNameSample);

}
