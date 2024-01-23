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
class GameScene : public IScene
{

public:

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

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;

	// ポーズ
	// テクスチャハンドル
	std::array<uint32_t, Pause::PauseTextureNo::kCountOfPauseTextureNo> pauseTextureHandles_;
	std::unique_ptr<Pause> pause_ = nullptr;

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Material> material_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 direction = { 1.0f, -1.0f, 0.0f};
	float intencity = 1.0f;

	//ブロックのテスト
	std::unique_ptr<Model> modelBlock_;
	//std::vector<std::unique_ptr<Block>> blocks_;
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

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// 惑星
	std::unique_ptr<Planet> planet_;
	std::unique_ptr<Model> planetModel_;

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

};
