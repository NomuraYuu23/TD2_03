#pragma once
#include "../../Engine/Audio/AudioManager.h"

enum GameAudioNameIndex {
	kGameAudioNameIndexBGM, // BGM
	kGameAudioNameIndexUnion1, // 合体1
	kGameAudioNameIndexUnion2, // 合体2
	kGameAudioNameIndexUnion3, // 合体3
	kGameAudioNameIndexPlayerShootRemove, // プレイヤー撃って戻る 
	kGameAudioNameIndexScrewRemove, // ねじ戻る
	kGameAudioNameIndexScrewShoot, // ねじ撃つ
	kGameAudioNameIndexMissionClear, // ミッションクリア
	kGameAudioNameIndexMissionOccurrrence, // ミッション更新
	kGameAudioNameIndexPlayerGravity, // プレイヤー重力
	kGameAudioNameIndexPauseChice, // ポーズメニュー選択
	kGameAudioNameIndexPauseDesicion, // ポーズメニュー決定
	kGameAudioNameIndexFlagGet, //旗入手
	kGameAudioNameIndexStart, //スタート
	kGameAudioNameIndexEnd, //終了
	kGameAudioNameIndexOfCount // 数
};

class GameAudioManager : public AudioManager
{

public: // メンバ関数

	~GameAudioManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="audioIndex">オーディオの名前</param>
	/// <returns>再生中のサウンドデータの番号</returns>
	void PlayWave(uint32_t audioIndex) override;

private: // メンバ変数

	// データ
	std::array<AudioData, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioNames_ = 
	{
		"BGM/ingame.wav",
		"Soundeffect/Game/block_union1.wav",
		"Soundeffect/Game/block_union2.wav",
		"Soundeffect/Game/block_union3.wav",
		"Soundeffect/Game/ingame_player_shootRemove.wav",
		"Soundeffect/Game/ingame_screw_remove.wav",
		"Soundeffect/Game/ingame_screw_shoot.wav",
		"Soundeffect/Game/mission_clear.wav",
		"Soundeffect/Game/mission_occurrence.wav",
		"Soundeffect/Game/player_gravity.wav",
		"Soundeffect/Common/desicion2.wav",
		"Soundeffect/Common/desicion1.wav",
		"Soundeffect/Game/ingame_flag_remove.wav",
		"Soundeffect/Game/ingame_start.wav",
		"Soundeffect/Game/ingame_finish.wav",
	};

};

