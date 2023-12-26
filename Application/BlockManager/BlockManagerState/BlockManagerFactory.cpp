#include "BlockManagerFactory.h"
#include <cassert>

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
	case BlockManagerState::kBlockManagerStateOfCount:
	default:
		assert(0);
		break;
	}

}
