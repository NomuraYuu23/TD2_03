#include "StartMigration.h"
#include "../../Engine/Math/Ease.h"

void StartMigration::Initialize(uint32_t textureHandle)
{

	startSprite_.reset(Sprite::Create(textureHandle, startPositon_, { 1.0f,1.0f,1.0f,1.0f }));
	startSprite_->SetSize(Vector2{ 1400.0f * 0.8f, 300.0f * 0.8f });
	startSprite_->SetIsInvisible(true);
	postionT_ = 0.0f;
	coolTime_ = 0.0f;
	isCooltime_ = false;
	fromTheStart_ = true;
	isEnd_ = false;
	isStart_ = false;
	startCount_ = 0;

}

void StartMigration::Start()
{
	if (++startCount_ == 2) {
		startSprite_->SetIsInvisible(false);
		isStart_ = true;
	}

}

void StartMigration::Update()
{

	if (isEnd_ || !isStart_) {
		return;
	}

	if (!isCooltime_) {
		float speed = 0.02f;
		Vector2 pos = { 0.0f,0.0f };
		postionT_ += speed;
		if (fromTheStart_) {
			if (postionT_ >= 1.0f) {
				postionT_ = 1.0f;
				isCooltime_ = true;
				fromTheStart_ = false;
			}
			pos = Ease::Easing(Ease::EaseName::EaseInCubic, startPositon_, middlePositon_, postionT_);
		}
		else {
			if (postionT_ >= 1.0f) {
				// 終了
				isEnd_ = true;
			}
			pos = Ease::Easing(Ease::EaseName::EaseOutCubic, middlePositon_, endPositon_, postionT_);
		}
		startSprite_->SetPosition(pos);
	}
	// クールタイム
	else {
		float speed = 1.0f / 60.0f;
		coolTime_ += speed;
		if (coolTime_ >= 2.0f) {
			isCooltime_ = false;
			postionT_ = 0.0f;
		}
	}


}

void StartMigration::Draw()
{

	startSprite_->Draw();

}
