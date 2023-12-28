#pragma once
#include "../Block/Block.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/2D/Sprite.h"
#include "../player.h"
#include <vector>
class Target
{
public:
	void Initialize(uint32_t textureHandle);
	void Update(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera,Player* player);
	void SpriteDraw();
	
	void ForchNearAnchor(std::vector<std::unique_ptr<Block>>* blockList, BaseCamera& camera,Player* player);

	bool IsTarget() { return isTarget_; };
	Block* GetTargetBlock() { return targetBlock_; };
	size_t GetNumTargetAnchor() { return numTargetAnchor_; };
private:
	bool isTarget_;//ターゲットしているか
	Block* targetBlock_;//対象ブロックのポインタ
	size_t numTargetAnchor_;//対象ブロックのどこをターゲットしているか
	std::unique_ptr<Sprite> anchor_;
};
