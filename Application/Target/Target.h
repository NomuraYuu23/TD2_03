#pragma once
#include "../Block/Block.h"
#include "../../Engine/Camera/BaseCamera.h"
#include <vector>
class Target
{
public:
	void Initialize();
	void Update(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera);
	void ForchNearAnchor(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera);

	bool IsTarget() { return isTarget_; };
	Block* GetTargetBlock() { return targetBlock_; };
	size_t GetNumTargetAnchor() { return numTargetAnchor_; };
private:
	bool isTarget_;//ターゲットしているか
	Block* targetBlock_;//対象ブロックのポインタ
	size_t numTargetAnchor_;//対象ブロックのどこをターゲットしているか
};
