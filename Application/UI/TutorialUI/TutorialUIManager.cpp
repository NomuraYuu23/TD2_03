#include "TutorialUIManager.h"
#include "../../../Engine/Math/Ease.h"

void TutorialUIManager::Initialize(const std::array<uint32_t, TutorialUITextureHandleIndex::kTutorialUITextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIInitPositions_[i] = UIs_[i]->GetPosition();
	}

	leftTutorialMissionStamp_.Initialize();
	rightTutorialMissionStamp_.Initialize();

	// ミッション更新中の色
	missionBeenUpdateColor_ = { 1.0f, 1.0f, 1.0f, 1.0f};
	// ミッション
	missionBeenUpdateFadeIn_ = false;

}

void TutorialUIManager::Update(const std::array<bool, 7>& isBeenMissionUpdate, const std::array<bool, 7>& isClearMission, float ratio)
{

	if (isClearMission[6]) {
		skipGageSizeStopUpdate_ = true;
		UIs_[kTutorialUIIndexSkipGage]->SetSize(Vector2{0.0f,0.0f});
	}

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIs_[i]->Update();
	}

	// ミッションの更新が入ったか
	for (size_t i = 0; i < isBeenMissionUpdate.size(); i++) {
		if (isBeenMissionUpdate[i]) {
			if (i % 2 == 0) {
				leftTutorialMissionStamp_.pressStamp_ = true;
				UIs_[kTutorialUIIndexMissionClearLeft]->SetIsInvisible(false);
				leftTutorialMissionStamp_.stampT_ = 0.0f;
			}
			else {
				rightTutorialMissionStamp_.pressStamp_ = true;
				UIs_[kTutorialUIIndexMissionClearRight]->SetIsInvisible(false);
				rightTutorialMissionStamp_.stampT_ = 0.0f;
			}
			audioManager_->PlayWave(kGameAudioNameIndexMissionClear);
		}
	}

	// 左更新
	LeftMissionUpdate();
	// 右更新
	RightMissionUpdate();
	// 更新
	MissionUpdate(isClearMission);

	// ミッションの更新を止める
	if (leftTutorialMissionStamp_.pressStamp_ && rightTutorialMissionStamp_.pressStamp_) {
		stopTheMission_ = true;
	}

	// スキップゲージ更新
	SkipGageUpde(ratio);

}

void TutorialUIManager::Draw()
{

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}

}

void TutorialUIManager::UIInitialize()
{

	Vector2 size = { 1000.0f, 1000.0f };
	Vector2 leftTop = { 0.0f, 0.0f };
	std::string jsonName = "TutorialUI";

	size = {1280.0f, 720.0f};
	UIs_[kTutorialUIIndexFrame] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexFrame]->Initialize(textureHandles_[kTutorialUITextureHandleIndexFrame], "Frame", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionFrameLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionFrameLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionFrame], "MissionFrameLeft", size, leftTop, jsonName);

	size = { 800.0f, 700.0f };
	UIs_[kTutorialUIIndexMissionClearLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionClearLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionClear], "MissionClearLeft", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionTextLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionTextLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionTextMove], "MissionTextLeft", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionFrameRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionFrameRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionFrame], "MissionFrameRight", size, leftTop, jsonName);

	size = { 800.0f, 700.0f };
	UIs_[kTutorialUIIndexMissionClearRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionClearRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionClear], "MissionClearRight", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionTextRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionTextRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionTextCamera], "MissionTextRight", size, leftTop, jsonName);

	size = { 900.0f, 125.0f };
	UIs_[kTutorialUIIndexSkipText] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipText]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipText], "SkipText", size, leftTop, jsonName);

	size = { 2.0f, 2.0f };
	UIs_[kTutorialUIIndexSkipGage] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipGage]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipGage], "SkipGage", size, leftTop, jsonName);
	UIs_[kTutorialUIIndexSkipGage]->SetAnchorPoint(Vector2{ 0.0f,0.0f });
	skipGageSize_ = UIs_[kTutorialUIIndexSkipGage]->GetSize();

	size = { 900.0f, 125.0f };
	UIs_[kTutorialUIIndexSkipGageFrame] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipGageFrame]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipGageFrame], "SkipGageFrame", size, leftTop, jsonName);

	size = { 320.0f, 256.0f };
	UIs_[kTutorialUIIndexStickL] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexStickL]->Initialize(textureHandles_[kTutorialUITextureHandleIndexStickL], "StickL", size, leftTop, jsonName);

	size = { 320.0f, 256.0f };
	UIs_[kTutorialUIIndexStickR] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexStickR]->Initialize(textureHandles_[kTutorialUITextureHandleIndexStickR], "StickR", size, leftTop, jsonName);

	size = { 280.0f, 192.0f };
	UIs_[kTutorialUIIndexButtonLB] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonLB]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonLB], "ButtonLB", size, leftTop, jsonName);

	size = { 280.0f, 192.0f };
	UIs_[kTutorialUIIndexButtonRB] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonRB]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonRB], "ButtonRB", size, leftTop, jsonName);

	size = { 450.0f, 450.0f };
	UIs_[kTutorialUIIndexButtonA] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonA]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonA], "ButtonA", size, leftTop, jsonName);

	size = { 450.0f, 450.0f };
	UIs_[kTutorialUIIndexButtonY] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonY]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonY], "ButtonY", size, leftTop, jsonName);

	UIs_[kTutorialUIIndexMissionClearLeft]->SetIsInvisible(true);
	UIs_[kTutorialUIIndexMissionClearRight]->SetIsInvisible(true);

	UIs_[kTutorialUIIndexButtonLB]->SetIsInvisible(true);
	UIs_[kTutorialUIIndexButtonRB]->SetIsInvisible(true);
	UIs_[kTutorialUIIndexButtonA]->SetIsInvisible(true);

}

void TutorialUIManager::NewMissionUpdate(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addStart = 1280.0f;

	// ミッションフレーム
	start = UIInitPositions_[kTutorialUIIndexMissionFrameLeft];
	end = UIInitPositions_[kTutorialUIIndexMissionFrameLeft];
	start.x += addStart;
	UIs_[kTutorialUIIndexMissionFrameLeft]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kTutorialUIIndexMissionTextLeft];
	end = UIInitPositions_[kTutorialUIIndexMissionTextLeft];
	start.x += addStart;
	UIs_[kTutorialUIIndexMissionTextLeft]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	// ミッションフレーム
	start = UIInitPositions_[kTutorialUIIndexMissionFrameRight];
	end = UIInitPositions_[kTutorialUIIndexMissionFrameRight];
	start.x += addStart;
	UIs_[kTutorialUIIndexMissionFrameRight]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kTutorialUIIndexMissionTextRight];
	end = UIInitPositions_[kTutorialUIIndexMissionTextRight];
	start.x += addStart;
	UIs_[kTutorialUIIndexMissionTextRight]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	// コントローラー
	start = UIInitPositions_[kTutorialUIIndexStickL];
	end = UIInitPositions_[kTutorialUIIndexStickL];
	start.x += addStart;
	UIs_[kTutorialUIIndexStickL]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexStickR];
	end = UIInitPositions_[kTutorialUIIndexStickR];
	start.x += addStart;
	UIs_[kTutorialUIIndexStickR]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonLB];
	end = UIInitPositions_[kTutorialUIIndexButtonLB];
	start.x += addStart;
	UIs_[kTutorialUIIndexButtonLB]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonRB];
	end = UIInitPositions_[kTutorialUIIndexButtonRB];
	start.x += addStart;
	UIs_[kTutorialUIIndexButtonRB]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonA];
	end = UIInitPositions_[kTutorialUIIndexButtonA];
	start.x += addStart;
	UIs_[kTutorialUIIndexButtonA]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

}

void TutorialUIManager::ClearMissionUpdate(float t)
{

	//ミッション位置
	Vector2 start = { 0.0f,0.0f };
	Vector2 end = { 0.0f,0.0f };
	float addEnd = 1280.0f;

	// ミッションフレーム
	start = UIInitPositions_[kTutorialUIIndexMissionFrameLeft];
	end = UIInitPositions_[kTutorialUIIndexMissionFrameLeft];
	end.x += addEnd;
	UIs_[kTutorialUIIndexMissionFrameLeft]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kTutorialUIIndexMissionTextLeft];
	end = UIInitPositions_[kTutorialUIIndexMissionTextLeft];
	end.x += addEnd;
	UIs_[kTutorialUIIndexMissionTextLeft]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	// ミッションフレーム
	start = UIInitPositions_[kTutorialUIIndexMissionFrameRight];
	end = UIInitPositions_[kTutorialUIIndexMissionFrameRight];
	end.x += addEnd;
	UIs_[kTutorialUIIndexMissionFrameRight]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));
	// ミッションテキスト
	start = UIInitPositions_[kTutorialUIIndexMissionTextRight];
	end = UIInitPositions_[kTutorialUIIndexMissionTextRight];
	end.x += addEnd;
	UIs_[kTutorialUIIndexMissionTextRight]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	// コントローラー
	start = UIInitPositions_[kTutorialUIIndexStickL];
	end = UIInitPositions_[kTutorialUIIndexStickL];
	end.x += addEnd;
	UIs_[kTutorialUIIndexStickL]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexStickR];
	end = UIInitPositions_[kTutorialUIIndexStickR];
	end.x += addEnd;
	UIs_[kTutorialUIIndexStickR]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonLB];
	end = UIInitPositions_[kTutorialUIIndexButtonLB];
	end.x += addEnd;
	UIs_[kTutorialUIIndexButtonLB]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonRB];
	end = UIInitPositions_[kTutorialUIIndexButtonRB];
	end.x += addEnd;
	UIs_[kTutorialUIIndexButtonRB]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));

	start = UIInitPositions_[kTutorialUIIndexButtonA];
	end = UIInitPositions_[kTutorialUIIndexButtonA];
	end.x += addEnd;
	UIs_[kTutorialUIIndexButtonA]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutCubic, start, end, t));


}

void TutorialUIManager::LeftStamp()
{

	float speed = 0.05f;

	leftTutorialMissionStamp_.stampT_ += speed;
	if (leftTutorialMissionStamp_.stampT_ >= 1.0f) {
		leftTutorialMissionStamp_.stampT_ = 1.0f;
		if (leftTutorialMissionStamp_.stampCooltime_ >= 1.0f) {
			leftTutorialMissionStamp_.stampCooltime_ = 0.0f;
			leftTutorialMissionStamp_.isStamped_ = true;
		}
		else {
			leftTutorialMissionStamp_.stampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 120.0f, 105.0f };
	Vector2 end = { 96.0f, 84.0f };
	UIs_[kTutorialUIIndexMissionClearLeft]->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, leftTutorialMissionStamp_.stampT_));

}

void TutorialUIManager::RightStamp()
{

	float speed = 0.05f;

	rightTutorialMissionStamp_.stampT_ += speed;
	if (rightTutorialMissionStamp_.stampT_ >= 1.0f) {
		rightTutorialMissionStamp_.stampT_ = 1.0f;
		if (rightTutorialMissionStamp_.stampCooltime_ >= 1.0f) {
			rightTutorialMissionStamp_.stampCooltime_ = 0.0f;
			rightTutorialMissionStamp_.isStamped_ = true;
		}
		else {
			rightTutorialMissionStamp_.stampCooltime_ += speed;
		}
	}

	// ミッションクリア
	Vector2 start = { 120.0f, 105.0f };
	Vector2 end = { 96.0f, 84.0f };
	UIs_[kTutorialUIIndexMissionClearRight]->SetSize(Ease::Easing(Ease::EaseName::EaseInBack, start, end, rightTutorialMissionStamp_.stampT_));

}

void TutorialUIManager::LeftMissionUpdate()
{

	// クリア
	if (leftTutorialMissionStamp_.pressStamp_ && !leftTutorialMissionStamp_.isStamped_) {
		// ハンコ
		LeftStamp();
	}

}

void TutorialUIManager::RightMissionUpdate()
{

	// クリア
	if (rightTutorialMissionStamp_.pressStamp_ && !rightTutorialMissionStamp_.isStamped_) {
		// ハンコ
		RightStamp();
	}

}

void TutorialUIManager::MissionUpdate(const std::array<bool, 7>& isClearMission)
{

	// クリア
	if (!missionBeenUpdateFadeIn_) {

		if (leftTutorialMissionStamp_.isStamped_ && rightTutorialMissionStamp_.isStamped_) {
			// コンプリート
			//if (isCompleteBlock) {
			//	blockMissionBeenUpdate_ = false;
			//	return;
			//}

			missionBeenUpdateColor_.w -= 0.05f;
			
			if (missionBeenUpdateColor_.w <= 0.0f) {

				missionBeenUpdateColor_.w = 0.0f;

				missionBeenUpdateFadeIn_ = true;

				UIs_[kTutorialUIIndexMissionClearLeft]->SetIsInvisible(true);
				UIs_[kTutorialUIIndexMissionClearRight]->SetIsInvisible(true);

				// テクスチャハンドルの変更
				MisiionTextChange(isClearMission);
				
				if (isClearMission[5]) {
					LastMisiion();
				}

				audioManager_->PlayWave(kGameAudioNameIndexMissionOccurrrence);
			}

			ClearMissionUpdate((1.0f - missionBeenUpdateColor_.w));
		}
	}
	// 新しいものが飛んでくる
	else {
		missionBeenUpdateColor_.w += 0.05f;
		if (missionBeenUpdateColor_.w >= 1.0f) {
			missionBeenUpdateColor_.w = 1.0f;
			missionBeenUpdateFadeIn_ = false;
			leftTutorialMissionStamp_.Initialize();
			rightTutorialMissionStamp_.Initialize();

			// ミッションクリア
			UIs_[kTutorialUIIndexMissionClearLeft]->SetPosition(UIInitPositions_[kTutorialUIIndexMissionClearLeft]);
			UIs_[kTutorialUIIndexMissionClearRight]->SetPosition(UIInitPositions_[kTutorialUIIndexMissionClearRight]);
			stopTheMission_ = false;
		}

		NewMissionUpdate(missionBeenUpdateColor_.w);

	}

	for (size_t i = kTutorialUIIndexMissionFrameLeft; i < kTutorialUIIndexSkipText; i++) {
		UIs_[i]->SetColor(missionBeenUpdateColor_);
	}
	for (size_t i = kTutorialUIIndexStickL; i < kTutorialUIIndexButtonY; i++) {
		UIs_[i]->SetColor(missionBeenUpdateColor_);
	}

}

void TutorialUIManager::MisiionTextChange(const std::array<bool, 7>& isClearMission)
{

	if (isClearMission[5]) {
		UIs_[kTutorialUIIndexMissionTextLeft]->SetTextureHandle(textureHandles_[kTutorialUITextureHandleIndexMissionTextGravity]);

		UIs_[kTutorialUIIndexButtonLB]->SetIsInvisible(true);

		UIs_[kTutorialUIIndexButtonA]->SetIsInvisible(false);

	}
	else if(isClearMission[3]){
		UIs_[kTutorialUIIndexMissionTextLeft]->SetTextureHandle(textureHandles_[kTutorialUITextureHandleIndexMissionTextLockOn]);
		UIs_[kTutorialUIIndexMissionTextRight]->SetTextureHandle(textureHandles_[kTutorialUITextureHandleIndexMissionTextShoot2]);
	
		UIs_[kTutorialUIIndexButtonRB]->SetIsInvisible(true);

		UIs_[kTutorialUIIndexButtonLB]->SetIsInvisible(false);

	}
	else{
		UIs_[kTutorialUIIndexMissionTextLeft]->SetTextureHandle(textureHandles_[kTutorialUITextureHandleIndexMissionTextShoot]);
		UIs_[kTutorialUIIndexMissionTextRight]->SetTextureHandle(textureHandles_[kTutorialUITextureHandleIndexMissionTextApproach]);
		
		UIs_[kTutorialUIIndexStickL]->SetIsInvisible(true);
		UIs_[kTutorialUIIndexStickR]->SetIsInvisible(true);

		UIs_[kTutorialUIIndexButtonRB]->SetIsInvisible(false);

	}

}

void TutorialUIManager::LastMisiion()
{

	UIs_[kTutorialUIIndexMissionFrameRight]->SetIsInvisible(true);
	UIs_[kTutorialUIIndexMissionClearRight]->SetIsInvisible(true);
	UIs_[kTutorialUIIndexMissionTextRight]->SetIsInvisible(true);

	Vector2 pos = {0.0f,0.0f};

	pos = { 640.0f, 550.0f };
	UIInitPositions_[kTutorialUIIndexMissionFrameLeft] = pos;
	pos = { 805.0f, 558.0f };
	UIInitPositions_[kTutorialUIIndexMissionClearLeft] = pos;
	pos = { 640.0f, 550.0f };
	UIInitPositions_[kTutorialUIIndexMissionTextLeft] = pos;

}

void TutorialUIManager::SkipGageUpde(float ratio)
{
	
	if (skipGageSizeStopUpdate_) {
		return;
	}

	Vector2 size = { skipGageSize_.x * ratio, skipGageSize_.y };

	UIs_[kTutorialUIIndexSkipGage]->SetSize(size);

	if (ratio >= 1.0f) {
		skipGageSizeStopUpdate_ = true;
	}

}

void TutorialMissionStamp::Initialize()
{

	// ハンコを押せ
	pressStamp_ = false;
	// ハンコが押された
	isStamped_ = false;
	// ハンコの媒介変数
	stampT_ = 0.0f;
	// ハンコのクールタイム
	stampCooltime_ = 0.0f;

}
