#include "BlockManagerFactory.h"
#include <cassert>
#include "BlockManagerStateInit/BlockManagerStateInit.h"

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
	case BlockManagerState::kBlockManagerStateOfCount:
	default:
		assert(0);
		break;
	}

	return newBlockManagerState;

}
