#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include "UI.h"
#include "../AudioManager/GameAudioManager.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexScrews, // ねじ
	kUITextureHandleIndexNumber, // 数字
	kUITextureHandleIndexSymbol, // 記号
	kUITextureHandleIndexFrame, // フレーム
	kUITextureHandleIndexMissionFrame, // ミッションフレーム
	kUITextureHandleIndexMissionText, // ミッションテキスト
	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexFrame, // フレーム
	kUIIndexRemainingScrewsIcon, // 残りねじアイコン
	kUIIndexRemainingScrewsCross, // 残りねじのx
	kUIIndexRemainingScrewsTensPlace, // 残りねじの数字10の位
	kUIIndexRemainingScrewsOnesPlace, // 残りねじの数字1の位
	kUIIndexTimerMinutes, // タイマー分
	kUIIndexTimerSecondsTensPlace, // タイマー秒10の位
	kUIIndexTimerSecondsOnesPlace, // タイマー秒1の位
	kUIIndexTimerColon, // タイマーコロン
	kUIIndexMissionFrame, // ミッションフレーム
	kUIIndexMissionText, // ミッションテキスト
	kUIIndexMissionNumTensPlace, // ミッション番号10の位
	kUIIndexMissionNumOnesPlace, // ミッション番号1の位
	kUIIndexMissionDenominatorTensPlace, // ミッション分母10の位
	kUIIndexMissionDenominatorOnesPlace, // ミッション分母1の位
	kUIIndexMissionNumeratorTensPlace, // ミッション分子10の位
	kUIIndexMissionNumeratorOnesPlace, // ミッション分子1の位
	kUIIndexOfCount
};

class UIManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles">テクスチャハンドル</param>
	/// <param name="energyMax">エナジー最大量</param>
	void Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="screwCount">ねじの数</param>
	/// <param name="energy">エナジー</param>
	void Update(uint32_t screwCount, uint32_t missionBlockCount, uint32_t blockCount, bool missionBeenUpdated, int timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// UIごとの初期化
	/// </summary>
	/// <param name="energyMax">エナジー最大量</param>
	void UIInitialize();

	/// <summary>
	/// ミッションアップデート
	/// </summary>
	void MissionUpdate(uint32_t missionBlockCount, uint32_t blockCount);

	/// <summary>
	/// ブロック更新
	/// </summary>
	void BlockCountUpdate(uint32_t blockCount);

	/// <summary>
	/// ミッションブロック更新
	/// </summary>
	void MissionBlockCountUpdate(uint32_t missionBlockCount);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

	// ミッション更新中
	bool missionBeenUpdate_;

	// ミッション更新中の色
	Vector4 missionBeenUpdateColor_;

	// ミッション
	bool missionBeenUpdateFadeIn_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

public:

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

};

