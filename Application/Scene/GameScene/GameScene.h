#pragma once

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../../Engine/Collision/CollisionManager.h"
#include "../../Pause/Pause.h"
#include "../../../Engine/base/ITextureHandleManager.h"
#include "../../player/player.h"
#include "../../Block/Block.h"
#include "../../Screw/Screw.h"
#include "../../Target/Target.h"
#include "../../Camera/FollowCamera/FollowCamera.h"
#include "../../../Engine/Collision/CollisionManager.h"
#include "../../UI/UIManager.h"
#include "../../AudioManager/GameAudioManager.h"
#include "../../Skydome/Skydome.h"
#include "../../Planet/Planet.h"
#include "../../Energy/Energy.h"
#include "../../Block/UFO.h"

#include "../../BlockManager/BlockManager.h"

#include "../../UFO/UFOManager.h"
#include "../../../Engine/3D/OutLineData.h"

#include "../../ShadowManager/ShadowManager.h"

#include "../../ClearMigration/ClearMigration.h"
#include "../../MissionData/MissionData.h"
#include "../../Rocket/Rocket.h"
class GameScene : public IScene
{

public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void GoToTheTitle();

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
	/// 影更新
	/// </summary>
	void ShadowUpdate();

private:

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// ライト
	Vector3 direction = { 1.0f, -1.0f, 0.0f};
	float intencity = 1.0f;

	//ブロックのテスト
	std::unique_ptr<Model> modelBlock_;
	std::unique_ptr<BlockManager> blockManager_;

	Target target_;
	
	std::unique_ptr<Player> player_;
	std::array<std::unique_ptr<Model>, PlayerPartIndex::kPlayerPartIndexOfCount> playerModels_;

	std::list<std::unique_ptr<Screw>> screws_;
	std::unique_ptr<Model> modelPlayer_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_;
	uint32_t cursorTextureHandle_;

	std::unique_ptr<CollisionManager> collisionManager_;

	// UIマネージャー
	std::unique_ptr<UIManager> uiManager_;
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> uiTextureHandles_;

	uint32_t shotUITextureHandle_[2];

	std::unique_ptr<Model> modelCircle_ = nullptr;
	// オーディオマネージャー
	std::unique_ptr<GameAudioManager> audioManager_;
	bool isDecreasingVolume = true;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// 惑星
	//std::unique_ptr<Planet> planet_;
	std::unique_ptr<Model> planetModel_;
	std::unique_ptr<Model> planetFlagModel_;
	std::vector<std::unique_ptr<Planet>> planets_;

	//std::unique_ptr<UFO> ufo_;
	std::vector<std::unique_ptr<UFO>> ufos_;
	std::unique_ptr<UFOManager> ufoManager_;

	std::unique_ptr<Model> modelScrew_;
	int firstScrewNum_ = 6;//最初に生成するネジの本数

	std::unique_ptr<Energy> energy_;

	float energyPoint_;//エネルギー量仮
	float energyMax_ = 2000.0f;//エネルギー最大量仮

	//アウトライン仮
	OutLineData outline_;

	size_t missionNum_;
	std::vector<int32_t> mission_;
	uint32_t whiteTextureHandle_;

	int gameTimer_;//受け渡し用秒単位時間
	float gameTimerFloat_;
	int timerMax_ = 300;

	std::unique_ptr<Model> sweatModel_;

	//影
	std::unique_ptr<ShadowManager> shadowManager_;
	std::unique_ptr<Model> shadowModel_;

	// クリア演出
	std::unique_ptr<ClearMigration> clearMigration_;
	uint32_t clearMigrationTextureHandle_;

	std::unique_ptr<Model> soilModel_;

	// ポーズ
	std::unique_ptr<Pause> pause_;
	std::array<uint32_t, Pause::PauseTextureIndex::kPausingTextureIndexOfCount> pauseTextureHandles_;

	// ミッションデータ
	MissionData* missionData_ = nullptr;
	uint32_t dropTextureHandle_;
	uint32_t arrowTextureHandle_;
	uint32_t lockonTextureHandle_;
	uint32_t stickeTextureHandle_[2];

	// ロケット
	std::unique_ptr<Rocket> rocket_;
	std::unique_ptr<Model> rocketModel_;

};
