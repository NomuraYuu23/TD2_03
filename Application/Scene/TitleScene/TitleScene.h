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

	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void SpriteRegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summa
	void SpriteApplyGlobalVariables();

private: // メンバ変数

	// スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	// テクスチャハンドル
	uint32_t titleTextureHandle_ = 0u;
	// ポジション
	Vector2 titlePosition_ = {};
	// サイズ
	Vector2 titleSize_ = {};

	// スプライト
	std::unique_ptr<Sprite> buttonSprite_ = nullptr;
	// テクスチャハンドル
	uint32_t buttonTextureHandle_ = 0u;
	// ポジション
	Vector2 buttonPosition_ = {};
	// サイズ
	Vector2 buttonSize_ = {};
	// 点滅用媒介変数
	float buttonAlphaT_;
	// 点滅用媒介変数速度
	float buttonAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool buttonItIncreaseAlphaT_;
	// 色
	Vector4 buttonColor_;

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

	//アウトライン仮
	OutLineData outline_;
};

