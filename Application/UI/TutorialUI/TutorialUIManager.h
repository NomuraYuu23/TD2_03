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

struct TutorialMissionStamp {

	// ハンコを押せ
	bool pressStamp_;
	// ハンコが押された
	bool isStamped_;
	// ハンコの媒介変数
	float stampT_;
	// ハンコのクールタイム
	float stampCooltime_;
	
	// 初期化
	void Initialize();

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
	void Update(const std::array<bool,7>& isBeenMissionUpdate, const std::array<bool, 7>& isClearMission, float ratio);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// UIごとの初期化
	/// </summary>
	void UIInitialize();

	/// <summary>
	/// 新しいミッション移動
	/// </summary>
	void NewMissionUpdate(float t);

	/// <summary>
	/// クリアミッション移動
	/// </summary>
	/// <param name="t"></param>
	void ClearMissionUpdate(float t);

	/// <summary>
	/// 左スタンプ
	/// </summary>
	void LeftStamp();

	/// <summary>
	/// 右スタンプ
	/// </summary>
	void RightStamp();

	/// <summary>
	/// 左のミッション更新
	/// </summary>
	void LeftMissionUpdate();

	/// <summary>
	/// 右のミッション更新
	/// </summary>
	void RightMissionUpdate();

	/// <summary>
	/// ミッション更新
	/// </summary>
	void MissionUpdate(const std::array<bool, 7>& isClearMission);

	/// <summary>
	/// ミッションテキスト変更
	/// </summary>
	void MisiionTextChange(const std::array<bool, 7>& isClearMission);

	/// <summary>
	/// 最後のミッション
	/// </summary>
	void LastMisiion();

	/// <summary>
	/// スキップゲージ更新
	/// </summary>
	void SkipGageUpde(float ratio);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, TutorialUITextureHandleIndex::kTutorialUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, TutorialUIIndex::kTutorialUIIndexOfCount> UIs_;

	// UI位置
	std::array<Vector2, TutorialUIIndex::kTutorialUIIndexOfCount> UIInitPositions_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

	// チュートリアルミッション
	TutorialMissionStamp leftTutorialMissionStamp_;
	TutorialMissionStamp rightTutorialMissionStamp_;

	// ミッション更新中の色
	Vector4 missionBeenUpdateColor_;
	// ミッション
	bool missionBeenUpdateFadeIn_;

	// ミッションを止める
	bool stopTheMission_ = false;

	// スキップゲージサイズ
	Vector2 skipGageSize_ = { 0.0f, 0.0f };
	// スキップゲージサイズの更新を止める
	bool skipGageSizeStopUpdate_ = false;

public:

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

	bool GetStopTheMission() { return stopTheMission_; }

};

