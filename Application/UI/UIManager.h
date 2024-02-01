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
	kUITextureHandleIndexBlockMissionText, // ミッションテキスト
	kUITextureHandleIndexMissionNum, // ミッション数字
	kUITextureHandleIndexMissionClear,// ミッションクリア
	kUITextureHandleIndexPointMissionText, // ポイントミッションテキスト
	kUITextureHandleIndexFlagColor, // ミッションの旗色
	kUITextureHandleIndexMissionComplete, // ミッションコンプリート

	kUITextureHandleIndexLockOnOperationSymble, // ロックオンシンボル
	kUITextureHandleIndexLockOnOperationText, // ロックオンテキスト
	kUITextureHandleIndexGravityOperationSymble, // 重力シンボル
	kUITextureHandleIndexGravityOperationText, // 重力テキスト

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

	kUIIndexMissionBlockFrame, // ブロックミッションフレーム
	kUIIndexMissionBlockText, // ブロックミッションテキスト
	kUIIndexMissionNumTensPlace, // ミッション番号10の位
	kUIIndexMissionNumOnesPlace, // ミッション番号1の位
	kUIIndexMissionDenominatorTensPlace, // ミッション分母10の位
	kUIIndexMissionDenominatorOnesPlace, // ミッション分母1の位
	kUIIndexMissionNumeratorTensPlace, // ミッション分子10の位
	kUIIndexMissionNumeratorOnesPlace, // ミッション分子1の位
	kUIIndexMissionBlockClear, // ブロックミッションクリア

	kUIIndexMissionPointFrame, // ポイントミッションフレーム
	kUIIndexMissionPointText, // ポイントミッションテキスト
	kUIindexMissionFlagColor, // ミッション旗色
	kUIIndexMissionPointClear, // ポイントミッションクリア

	kUIIndexLockOnOperationSymble, // ロックオンシンボル
	kUIIndexLockOnOperationText, // ロックオンテキスト
	kUIIndexGravityOperationSymble, // 重力シンボル
	kUIIndexGravityOperationText, // 重力テキスト

	kUIIndexOfCount
};

struct UIManagerUpdateDesc {

	int timer; // 時間
	uint32_t screwCount; // ねじ
	
	int32_t missionBlockCount;// ミッションブロックカウント
	int blockCount; // ブロックカウント
	bool missionBlockBeenUpdated; // ブロックミッション更新か

	size_t missionNumPoint_;// ミッションポイントナンバー
	bool missionPointBeenUpdated; // ポイントミッション更新か

	bool isCompleteBlock; // ブロックのミッションがすべてクリアされたか
	bool isCompletePoint; //指定位置のミッションがすべてクリアされたか

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
	void Update(const UIManagerUpdateDesc& uiManagerUpdateDesc);

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
	void BlockMissionUpdate(uint32_t missionBlockCount, uint32_t blockCount, bool isCompleteBlock);

	/// <summary>
	/// ブロック更新
	/// </summary>
	void BlockCountUpdate(uint32_t blockCount);

	/// <summary>
	/// ミッションブロック更新
	/// </summary>
	void MissionBlockCountUpdate(uint32_t missionBlockCount);

	/// <summary>
	/// 新しいミッション移動
	/// </summary>
	void BlockNewMissionMove(float t);

	/// <summary>
	/// クリアミッション移動
	/// </summary>
	/// <param name="t"></param>
	void BlockClearMissionUpdate(float t);

	/// <summary>
	/// スタンプ
	/// </summary>
	void BlockStamp();

	/// <summary>
	/// ミッションアップデート
	/// </summary>
	void PointMissionUpdate(bool isCompletePoint, size_t num);

	/// <summary>
	/// スタンプ
	/// </summary>
	void PointStamp();

	/// <summary>
	/// クリアミッション移動
	/// </summary>
	void PointClearMissionUpdate(float t);

	/// <summary>
	/// 新しいミッション移動
	/// </summary>
	void PointNewMissionMove(float t);

	/// <summary>
	/// ミッション旗色
	/// </summary>
	void MissionPointNumUpdate(size_t num);


private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

	// UI位置
	std::array<Vector2, UIIndex::kUIIndexOfCount> UIInitPositions_;

	// ブロック
	// ミッション更新中
	bool blockMissionBeenUpdate_;
	// ミッション更新中の色
	Vector4 blockMissionBeenUpdateColor_;
	// ミッション
	bool blockMissionBeenUpdateFadeIn_;
	// ハンコが押された
	bool IsBlockStamped_;
	// ハンコの媒介変数
	float blockStampT_;
	// ハンコのクールタイム
	float blockStampCooltime_;

	// ポイント
	// ミッション更新中
	bool pointMissionBeenUpdate_;
	// ミッション更新中の色
	Vector4 pointMissionBeenUpdateColor_;
	// ミッション
	bool pointMissionBeenUpdateFadeIn_;
	// ハンコが押された
	bool IsPointStamped_;
	// ハンコの媒介変数
	float pointStampT_;
	// ハンコのクールタイム
	float pointStampCooltime_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

public:

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

};

