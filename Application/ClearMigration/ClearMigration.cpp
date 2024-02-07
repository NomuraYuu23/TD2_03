#include "ClearMigration.h"
#include "../../Engine/Math/Ease.h"

void ClearMigration::Initialize(uint32_t textureHandle)
{

	finishSprite_.reset(Sprite::Create(textureHandle, startPositon_, {1.0f,1.0f,1.0f,1.0f}));
	finishSprite_->SetSize(Vector2{ 1400.0f * 0.8f, 300.0f * 0.8f });
	finishSprite_->SetIsInvisible(true);
	postionT_ = 0.0f;
	coolTime_ = 0.0f;
	isCooltime_ = false;
	fromTheStart_ = true;
	isEnd_ = false;
	isStart_ = false;
}

void ClearMigration::Stert()
{
	finishSprite_->SetIsInvisible(false);
	isStart_ = true;
}

void ClearMigration::Update()
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
				audioManager_->PlayWave(kGameAudioNameIndexEnd);
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
		finishSprite_->SetPosition(pos);
	}
	// クールタイム
	else {
		float speed = 1.0f / 60.0f;
		coolTime_ += speed;
		if (coolTime_ >= 1.0f) {
			isCooltime_ = false;
			postionT_ = 0.0f;
		}
	}

}

void ClearMigration::Draw()
{

	finishSprite_->Draw();

}
