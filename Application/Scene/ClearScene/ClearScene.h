#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../AudioManager/ClearAudioManager.h"

#include "../../Skydome/Skydome.h"
#include "../../TitleObj/titleObj.h"
#include "../../ClearObj/ClearPlayer.h"

class ClearScene : public IScene
{

public: // メンバ関数

	~ClearScene();

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

	/// <summary>
	/// ランクスタンプ
	/// </summary>
	void RankStamp();

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void RankColorRegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summa
	void RankColorApplyGlobalVariables();

	void RankSColorAnimation();

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
	std::unique_ptr<ClearAudioManager> audioManager_;

	bool isDecreasingVolume = true;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;
	bool isDrawSkydome_ = true;

	// プレイヤー
	//std::unique_ptr<TitleObj> player_;
	//std::unique_ptr<Model> playerModel_;
	std::unique_ptr<ClearPlayer> player_;
	std::array<std::unique_ptr<Model>, PlayerPartIndex::kPlayerPartIndexOfCount> playerModels_;

	// ねじ
	std::array<std::unique_ptr<TitleObj>, 3> screws_;
	std::unique_ptr<Model> screwModel_;

	//アウトライン仮
	OutLineData outline_;

	// ミッション表示
	std::unique_ptr<Sprite> missionSprite_ = nullptr;
	uint32_t missionTextureHandle_ = 0u;
	Vector2 missionPosition_ = {};
	Vector2 missionSize_ = {};

	std::unique_ptr<Sprite> leftSprite_ = nullptr;
	Vector2 leftPosition_ = {};
	Vector2 leftSize_ = {};
	std::unique_ptr<Sprite> leftTenSprite_ = nullptr;
	Vector2 leftTenPosition_ = {};
	Vector2 leftTenSize_ = {};
	std::unique_ptr<Sprite> rightSprite_ = nullptr;
	Vector2 rightPosition_ = {};
	Vector2 rightSize_ = {};
	std::unique_ptr<Sprite> rightTenSprite_ = nullptr;
	Vector2 rightTenPosition_ = {};
	Vector2 rightTenSize_ = {};
	uint32_t numTextureHandle_ = 0u;

	size_t missionClearCount_;//カウント用
	size_t missionClearNum_;//クリアした数
	int32_t changeNumInterval_;
	int32_t frameCount_;
	size_t missionMax_;//最大ミッション数
	bool isEndCountUp_;//カウントが終了したか

	std::unique_ptr<Sprite> frameSprite_ = nullptr;
	Vector2 framePosition_ = {};
	Vector2 frameSize_ = {};
	uint32_t frameTextureHandle_ = 0u;

	std::unique_ptr<Sprite> toTitleSprite_ = nullptr;
	Vector2 toTitlePosition_ = {};
	Vector2 toTitleSize_ = {};
	uint32_t toTitleTextureHandle_ = 0u;

	std::unique_ptr<Sprite> lineSprite_ = nullptr;
	Vector2 linePosition_ = {};
	Vector2 lineSize_ = {};
	uint32_t lineTextureHandle_ = 0u;

	std::unique_ptr<Sprite> line2Sprite_ = nullptr;
	Vector2 line2Position_ = {};
	Vector2 line2Size_ = {};

	std::unique_ptr<Sprite> line3Sprite_ = nullptr;
	Vector2 line3Position_ = {};
	Vector2 line3Size_ = {};

	std::unique_ptr<Sprite> connectSprite_ = nullptr;
	Vector2 connectPosition_ = {};
	Vector2 connectSize_ = {};
	std::unique_ptr<Sprite> connectTenSprite_ = nullptr;
	Vector2 connectTenPosition_ = {};
	Vector2 connectTenSize_ = {};
	std::unique_ptr<Sprite> connectHandSprite_ = nullptr;
	Vector2 connectHandPosition_ = {};
	Vector2 connectHandSize_ = {};

	std::unique_ptr<Sprite> connectTextSprite_ = nullptr;
	Vector2 connectTextPosition_ = {};
	Vector2 connectTextSize_ = {};

	uint32_t clearRank_;
	std::unique_ptr<Sprite> rankTextSprite_ = nullptr;
	Vector2 rankTextPosition_ = {};
	Vector2 rankTextSize_ = {};
	std::array<uint32_t, 4> rankTextureHandles_;
	float stampT_ = 0.0f;
	float stampCooltime_ = 0.0f;
	bool isStamped_ = false;

	// 色
	std::array<Vector3, 4> rankColors_;

};

