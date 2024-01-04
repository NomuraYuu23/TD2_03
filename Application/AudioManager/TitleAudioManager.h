#pragma once
#include "../../Engine/Audio/AudioManager.h"

enum TitleAudioNameIndex {
	kTitleAudioNameIndexDecision, // 決定
	kTitleAudioNameIndexReturn, // 戻る
	kTitleAudioNameIndexBGM, // BGM
	kTitleAudioNameIndexOfCount // 数
};

class TitleAudioManager : public AudioManager
{


public: // メンバ関数

	~TitleAudioManager();

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
	std::array<AudioData, TitleAudioNameIndex::kTitleAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, TitleAudioNameIndex::kTitleAudioNameIndexOfCount> audioNames_ =
	{
		"Soundeffect/UI_decision.wav",
		"Soundeffect/UI_return.wav",
		"BGM/title_BGM.wav",
	};

};

