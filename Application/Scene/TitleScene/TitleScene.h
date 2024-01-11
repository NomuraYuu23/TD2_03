#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../AudioManager/TitleAudioManager.h"

class TitleScene : public IScene
{

public: // メンバ関数

	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// マテリアルクリエイト
	/// </summary>
	void MaterialCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	void LowerVolumeBGM();

private: // メンバ変数

	// スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	// テクスチャハンドル
	uint32_t titleTextureHandle_ = 0u;

	// スプライト
	std::unique_ptr<Sprite> buttonSprite_ = nullptr;
	// テクスチャハンドル
	uint32_t buttonTextureHandle_ = 0u;

	// オーディオマネージャー
	std::unique_ptr<TitleAudioManager> audioManager_;


	bool isDecreasingVolume = true;

};

