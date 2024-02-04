#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "../UI.h"
#include "../../AudioManager/GameAudioManager.h"

enum TutorialUITextureHandleIndex {
	kTutorialUITextureHandleIndexFrame, // フレーム
	kTutorialUITextureHandleIndexMissionFrame, // ミッションフレーム
	kTutorialUITextureHandleIndexMissionClear,// ミッションクリア

	kTutorialUITextureHandleIndexMissionTextMove, // ポイントミッションテキスト1
	kTutorialUITextureHandleIndexMissionTextCamera, // ポイントミッションテキスト2
	kTutorialUITextureHandleIndexMissionTextShoot, // ポイントミッションテキスト3
	kTutorialUITextureHandleIndexMissionTextApproach, // ポイントミッションテキスト4
	kTutorialUITextureHandleIndexMissionTextLockOn, // ポイントミッションテキスト5
	kTutorialUITextureHandleIndexMissionTextShoot2, // ポイントミッションテキスト6
	kTutorialUITextureHandleIndexMissionTextGravity, // ポイントミッションテキスト7

	kTutorialUITextureHandleIndexSkipText, // スキップテキスト
	kTutorialUITextureHandleIndexSkipGage, // スキップゲージ
	kTutorialUITextureHandleIndexSkipGageFrame, // スキップゲージフレーム

	kTutorialUITextureHandleIndexStickL, // スティックL
	kTutorialUITextureHandleIndexStickR, // スティックR
	kTutorialUITextureHandleIndexButtonLB, // ボタンLB
	kTutorialUITextureHandleIndexButtonRB, // ボタンRB
	kTutorialUITextureHandleIndexButtonA, // ボタンA
	kTutorialUITextureHandleIndexButtonY, // ボタンY

	kTutorialUITextureHandleIndexOfCount
};

enum TutorialUIIndex {
	kTutorialUIIndexFrame, // フレーム

	kTutorialUIIndexMissionFrameLeft, // ミッションフレーム左
	kTutorialUIIndexMissionClearLeft, // ミッションクリア左
	kTutorialUIIndexMissionTextLeft, // ミッションテキスト左
	kTutorialUIIndexMissionFrameRight, // ミッションフレーム右
	kTutorialUIIndexMissionClearRight, // ミッションクリア右
	kTutorialUIIndexMissionTextRight, // ミッションテキスト右

	kTutorialUIIndexSkipText, // スキップテキスト
	kTutorialUIIndexSkipGage, // スキップゲージ
	kTutorialUIIndexSkipGageFrame, // スキップゲージフレーム

	kTutorialUIIndexStickL, // スティックL
	kTutorialUIIndexStickR, // スティックR
	kTutorialUIIndexButtonLB, // ボタンLB
	kTutorialUIIndexButtonRB, // ボタンRB
	kTutorialUIIndexButtonA, // ボタンA
	kTutorialUIIndexButtonY, // ボタンY

	kTutorialUIIndexOfCount
};

class TutorialUIManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles">テクスチャハンドル</param>
	void Initialize(const std::array<uint32_t, TutorialUITextureHandleIndex::kTutorialUITextureHandleIndexOfCount>& textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// UIごとの初期化
	/// </summary>
	void UIInitialize();

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, TutorialUITextureHandleIndex::kTutorialUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, TutorialUIIndex::kTutorialUIIndexOfCount> UIs_;

	// UI位置
	std::array<Vector2, TutorialUIIndex::kTutorialUIIndexOfCount> UIInitPositions_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

public:

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

};

