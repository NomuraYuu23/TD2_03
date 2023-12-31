#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../player.h"
/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	// デバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	std::vector<Model*> colliderModels = { colliderSphereModel_.get(),colliderBoxModel_.get(),colliderBoxModel_.get() };
	colliderDebugDraw_->Initialize(colliderModels, colliderMaterial_.get());

	pause_ = std::make_unique<Pause>();
	pause_->Initialize(pauseTextureHandles_);

	// ビュープロジェクション
	TransformStructure baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);
	
	isDebugCameraActive_ = true;

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	model_.reset(Model::Create("Resources/default/", "Ball.obj", dxCommon_, textureHandleManager_.get()));
	material_.reset(Material::Create());
	material_->Initialize();
	TransformStructure uvTransform = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f},
	};
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	material_->Update(uvTransform, color, PhongReflection, 100.0f);

	worldTransform_.Initialize();

	modelBlock_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_, textureHandleManager_.get()));
	std::unique_ptr<Block> block;
	block.reset(new Block);
	block->Initialize();
	block->SetIsCenter(true);
	block->SetIsConnect(true);
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));
	
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({2.0f,0.0f,60.0f});
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 60.0f,0.0f,-2.0f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));
	

	player_.reset(new Player);
	player_->Initialize();
	colliderDebugDraw_->AddCollider(player_->GetCollider());
	colliderDebugDraw_->AddCollider(player_->GetMagnet()->GetCollider());
	
	for (int index = 0; index < 4; index++) {
		std::unique_ptr<Screw> screw;
		screw.reset(new Screw);
		screw->Initialize();
		screw->SetPlayer(player_.get());
		screws_.push_back(std::move(screw));
	}

	player_->SetScrew(&screws_);
	//player_->SetViewProjection(camera_);

	target_.Initialize(cursorTextureHandle_,shotUITextureHandle_);
	followCamera_.reset(new FollowCamera);
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetViewProjection(*followCamera_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(){
	ImguiDraw();
	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3Calc::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end();block++) {
		(*block)->Update();
	}
	for (std::vector<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Update();
	}
	target_.Update(&blocks_,*followCamera_.get(),player_.get());
	player_->Update(target_.GetTargetBlock(), target_.GetNumTargetAnchor());

	collisionManager_->ListClear();
	collisionManager_->ListRegister(player_->GetCollider());
	collisionManager_->ListRegister(player_->GetMagnet()->GetCollider());
	collisionManager_->ListRegister(blocks_[0]->GetCollider());
	collisionManager_->ListRegister(blocks_[1]->GetCollider());
	collisionManager_->ListRegister(blocks_[2]->GetCollider());
	collisionManager_->CheakAllCollision();
	camera_.Update();

	followCamera_->Update();
	camera_ = static_cast<BaseCamera>(*followCamera_.get());


	// デバッグカメラ
	DebugCameraUpdate();
	
	// デバッグ描画
	colliderDebugDraw_->Update();
	
	//パーティクル
	particleManager_->Update(debugCamera_->GetTransformMatrix());

	// ポーズ機能
	pause_->Update();

	// タイトルへ行く
	GoToTheTitle();

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());
	//3Dオブジェクトはここ

	//model_->Draw(worldTransform_, camera_, material_.get());
	
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
	}

	for (std::vector<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
	}
	player_->Draw(model_.get(), camera_);

#ifdef _DEBUG

	// デバッグ描画
	colliderDebugDraw_->Draw(camera_);

#endif // _DEBUG

	Model::PostDraw();

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), camera_.GetViewProjectionMatrix());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	// パーティクルはここ
	particleManager_->Draw();

	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());
	

	//背景
	//前景スプライト描画
	pause_->Draw();

	target_.SpriteDraw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){
#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::DragFloat3("direction", &direction.x, 0.1f);
	ImGui::DragFloat("i", &intencity, 0.01f);
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void GameScene::GoToTheTitle()
{

	if (pause_->GoToTheTitle()) {
		sceneNo = kTitle;
	}

}

void GameScene::ModelCreate()
{

	colliderSphereModel_.reset(Model::Create("Resources/TD2_November/collider/sphere/", "sphere.obj", dxCommon_,textureHandleManager_.get()));
	colliderBoxModel_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_, textureHandleManager_.get()));
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

}

void GameScene::MaterialCreate()
{

	colliderMaterial_.reset(Material::Create());

}

void GameScene::TextureLoad()
{

	// ポーズ
	pauseTextureHandles_ = {
		TextureManager::Load("Resources/TD2_November/pause/pausing.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/TD2_November/pause/goToTitle.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/TD2_November/pause/returnToGame.png", dxCommon_,textureHandleManager_.get()),
	};
	cursorTextureHandle_ = TextureManager::Load("Resources/ingame_target.png", dxCommon_, textureHandleManager_.get());
	shotUITextureHandle_[0] = TextureManager::Load("Resources/ingame_ui_RB.png", dxCommon_, textureHandleManager_.get());
	shotUITextureHandle_[1] = TextureManager::Load("Resources/ingame_ui_RB_remove.png", dxCommon_, textureHandleManager_.get());
}
