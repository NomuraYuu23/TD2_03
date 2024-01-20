#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../AudioManager/TitleAudioManager.h"

#include "../../Skydome/Skydome.h"
#include "../../TitleObj/TitleObj.h"

class TitleScene : public IScene
{

public: // メンバ関数

	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

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

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;
	bool isDrawSkydome_ = true;

	// プレイヤー
	std::unique_ptr<TitleObj> player_;
	std::unique_ptr<Model> playerModel_;

	// ねじ
	std::array<std::unique_ptr<TitleObj>, 3> screws_;
	std::unique_ptr<Model> screwModel_;


};

