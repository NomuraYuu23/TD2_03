#pragma once
#include "../../Engine/Input/Input.h"
#include "../../Engine/2D/Sprite.h"
#include <array>
#include "../AudioManager/GameAudioManager.h"

/// <summary>
/// ポーズ
/// </summary>
class Pause
{

public: // サブクラス

	// ポーズメニュー
	enum PauseMenu{
		kPauseMenuReturnToGame,    // ゲームに戻る
		kPauseMenuRestart,    // リスタート
		kPauseMenuGoToTitle,       // タイトルへ
		kCountOfPauseMenu // 使用不可
	};

	// ポーズで使うテクスチャ番号
	enum PauseTextureIndex {
		kPausingTextureIndexFrame, // ポーズ中
		kPausingTextureIndexReturnToGame, // ゲームに戻る
		kPausingTextureIndexRestart, // リスタート
		kPausingTextureIndexGoToTitle, // タイトルへ
		kPausingTextureIndexArrow, // 矢印
		kPausingTextureIndexChoice, // 選択
		kPausingTextureIndexBackGround, // 背景
		kPausingTextureIndexOfCount // 使用不可
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<uint32_t, PauseTextureIndex::kPausingTextureIndexOfCount>& textureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズ画面の切り替え
	/// </summary>
	void PoseSwitching();

private: // メンバ関数

	/// <summary>
	/// ポーズメニュー操作
	/// </summary>
	void PauseMenuOperation();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void PauseMenuGoToTitle();

	/// <summary>
	/// ゲームに戻る
	/// </summary>
	void PauseMenuReturnToGame();

	/// <summary>
	/// リスタート
	/// </summary>
	void PauseMenuRestart();

	/// <summary>
	/// スティック入力
	/// </summary>
	void InputStick();

public: // アクセッサ

	bool IsPause() { return isPause_; }

	bool GoToTheTitle() { return goToTheTitle_; }

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

private: // メンバ変数

	// 入力
	Input* input_ = nullptr;

	// ポーズしているか
	bool isPause_;
	// ポーズメニューでどこを選択しているか
	int pauseMenuSelect_;

	// タイトルへ戻るか
	bool goToTheTitle_;

	// テクスチャハンドル
	std::array<uint32_t, PauseTextureIndex::kPausingTextureIndexOfCount> textureHandles_;

	// タイトルへの位置
	float goToTitlePositionY_;
	// ゲームへの位置
	float returnToGamePositionY_;
	// リスタートの位置
	float restartPositionY_;

	// スティック
	float stickY_;

	// スティッククールタイム
	float stickColltime_;

	// オーディオマネージャー
	GameAudioManager* audioManager_;

private: // メンバ変数(スプライト)

	// ポーズ中
	std::unique_ptr<Sprite> frameSprite_;
	// タイトルへ
	std::unique_ptr<Sprite> goToTitleSprite_;
	// ゲームに戻る
	std::unique_ptr<Sprite> returnToGameSprite_;
	// リスタート
	std::unique_ptr<Sprite> restartSprite_;
	// 矢印
	std::unique_ptr<Sprite> arrowSprite_;
	// 選択
	std::unique_ptr<Sprite> choiceSprite_;
	// 背景
	std::unique_ptr<Sprite> backGroundSprite_;

};

