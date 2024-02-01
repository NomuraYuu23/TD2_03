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
	void Initialize(uint32_t textureHandle,uint32_t textureHandle2[2],uint32_t arrowTextureHandle,uint32_t lockonTextureHandle,uint32_t stickTextureHandle[2]);
	void Update(std::vector<Block*>* blockList, BaseCamera& camera,Player* player, std::list<std::unique_ptr<Screw>>* screwList);
	void SpriteDraw();
	
	void ForchNearAnchor(std::vector<Block*>* blockList, BaseCamera& camera,Player* player, std::list<std::unique_ptr<Screw>>* screwList);
	void ForchNearOneMore(std::vector<Block*>* blockList, BaseCamera& camera, Player* player, std::list<std::unique_ptr<Screw>>* screwList,int lr);
	bool AliveCheck(std::vector<Block*>* blockList);//ターゲット固定時に指定のブロックが生きているかを確認する用
	bool IsTarget() { return isTarget_; };
	bool IsLockedChange() { return isLockedChane_; };
	Block* GetTargetBlock() { return targetBlock_; };
	size_t GetNumTargetAnchor() { return numTargetAnchor_; };
private:
	bool isTarget_;//ターゲットしているか
	Block* targetBlock_;//対象ブロックのポインタ
	size_t numTargetAnchor_;//対象ブロックのどこをターゲットしているか
	std::unique_ptr<Sprite> anchor_;
	std::unique_ptr<Sprite> ui_;
	std::unique_ptr<Sprite> leftArrow_;
	std::unique_ptr<Sprite> rightArrow_;
	std::unique_ptr<Sprite> modeText_;
	std::unique_ptr<Sprite> leftStick_;
	std::unique_ptr<Sprite> rightStick_;
	float leftAnimation_=0;
	float rightAnimation_=0;
	int uiNum_=0;
	uint32_t textureHandle_[2];
	float targetRange_ = 100.0f;
	bool isReleseButton_;//長押し制限用
	bool isLockedChane_;//ターゲットが固定されているか
	float modeAlpha_=1.0f;
	float alphaDirection_=-1.0f;
	Vector2 modePosition_ = {0};
	Vector2 modeSize_ = {0};
	int stickAnimation_=0;
	int stickAnimationFrame_=0;
};
