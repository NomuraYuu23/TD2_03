#pragma once
#include "../../Engine/Audio/AudioManager.h"

enum TitleAudioNameIndex {
	kTitleAudioNameIndexBGM, // BGM
	kTitleAudioNameIndexDecision, // 決定
	kTitleAudioNameIndexDecision2, // 決定2
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
		"BGM/title.wav",
		"Soundeffect/Common/desicion1.wav",
		"Soundeffect/Common/desicion2.wav",
	};

};

