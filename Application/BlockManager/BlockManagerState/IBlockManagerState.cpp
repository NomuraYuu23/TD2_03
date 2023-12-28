#include "IBlockManagerState.h"
#include "../BlockManager.h"

uint32_t IBlockManagerState::blockManagerStateNo_ = 0;

BlockManager* IBlockManagerState::blockManager_ = nullptr;
