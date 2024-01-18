#pragma once
#include "../Block/Block.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/2D/Sprite.h"
#include "../Player/player.h"
#include "../Screw/Screw.h"
#include <vector>
#include <list>
class Target
{
public:
	void Initialize(uint32_t textureHandle,uint32_t textureHandle2[2]);
	void Update(std::vector<Block*>* blockList, BaseCamera& camera,Player* player, std::list<std::unique_ptr<Screw>>* screwList);
	void SpriteDraw();
	
	void ForchNearAnchor(std::vector<Block*>* blockList, BaseCamera& camera,Player* player, std::list<std::unique_ptr<Screw>>* screwList);

	bool IsTarget() { return isTarget_; };
	Block* GetTargetBlock() { return targetBlock_; };
	size_t GetNumTargetAnchor() { return numTargetAnchor_; };
private:
	bool isTarget_;//ターゲットしているか
	Block* targetBlock_;//対象ブロックのポインタ
	size_t numTargetAnchor_;//対象ブロックのどこをターゲットしているか
	std::unique_ptr<Sprite> anchor_;
	std::unique_ptr<Sprite> ui_;
	int uiNum_=0;
	uint32_t textureHandle_[2];
	float targetRange_ = 100.0f;
};
