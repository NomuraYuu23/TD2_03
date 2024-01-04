#pragma once
#include "../../Engine/Audio/AudioManager.h"

enum GameAudioNameIndex {
	kGameAudioNameIndexHipdrop, // ヒップドロップ
	kGameAudioNameIndexPlayerShootRemove, // プレイヤー撃って戻る 
	kGameAudioNameIndexScrewRemove, // ねじ戻る
	kGameAudioNameIndexScrewShoot, // ねじ撃つ
	kGameAudioNameIndexUnion, // 合体
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
		"Soundeffect/ingame_hipdrop.wav",
		"Soundeffect/ingame_player_shootRemove.wav",
		"Soundeffect/ingame_screw_remove.wav",
		"Soundeffect/ingame_screw_shoot.wav",
		"Soundeffect/ingame_union.wav",
	};


};

