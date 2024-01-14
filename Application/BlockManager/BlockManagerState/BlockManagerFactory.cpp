#include "BlockManagerFactory.h"
#include <cassert>
#include "BlockManagerStateInit/BlockManagerStateInit.h"
#include "BlockManagerStatePattern1/BlockManagerStatePattern1.h"

BlockManagerFactory* BlockManagerFactory::GetInstance()
{
	static BlockManagerFactory instance;
	return &instance;
}

IBlockManagerState* BlockManagerFactory::CreateBlockManagerState(uint32_t BlockManagerStateName)
{
	
	IBlockManagerState* newBlockManagerState = nullptr;

	switch (BlockManagerStateName)
	{
	case BlockManagerState::kBlockManagerStateInit:
		newBlockManagerState = new BlockManagerStateInit();
		break;
	case BlockManagerState::kBlockManagerState1:
		newBlockManagerState = new BlockManagerStatePattern1();
		break;
	case BlockManagerState::kBlockManagerStateOfCount:
	default:
		assert(0);
		break;
	}

	return newBlockManagerState;

}
