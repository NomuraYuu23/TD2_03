#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../player.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Screw";

	globalVariables->AddItem(groupName, "FirstScrewNum", firstScrewNum_);
	firstScrewNum_ = globalVariables->GetIntValue(groupName, "FirstScrewNum");
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
	
	isDebugCameraActive_ = false;

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
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));
	
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({2.0f,0.0f,60.0f});
	block->SetVelocity({0.0f,0.0f,-0.1f});
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 2.0f,0.0f,90.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 60.0f,0.0f,20.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));
	
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ -10.0f,0.0f,150.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ -30.0f,0.0f,200.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	//01/10仮追加分
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 2.0f,0.0f,250.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 2.0f,0.0f,300.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ -20.0f,0.0f,310.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ -10.0f,0.0f,350.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));

	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ -30.0f,0.0f,450.0f });
	block->SetVelocity({ 0.0f,0.0f,-0.1f });
	colliderDebugDraw_->AddCollider(block->GetCollider());
	blocks_.push_back(std::move(block));



	player_.reset(new Player);
	player_->Initialize();
	colliderDebugDraw_->AddCollider(player_->GetCollider());
	colliderDebugDraw_->AddCollider(player_->GetMagnet()->GetCollider());
	
	modelScrew_.reset(Model::Create("Resources/screw_model", "screw.obj", dxCommon_, textureHandleManager_.get()));
	for (int index = 0; index < firstScrewNum_; index++) {
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

	modelCircle_.reset(Model::Create("Resources/Circle/", "Circle.obj", dxCommon_, textureHandleManager_.get()));
	player_->SetCircle(modelCircle_.get());

	//UIマネージャー
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 惑星
	planet_ = std::make_unique<Planet>();
	planet_->Initialize(planetModel_.get());

	std::unique_ptr<UFO> ufo_;
	ufo_.reset(new UFO);
	ufo_->Initialize();
	ufo_->SetCircle(modelCircle_.get());
	ufo_->SetWorldPosition({ -10.0f,12.0f,150.0f });
	ufo_->SetVelocity({ 0.0f,0.0f,-0.1f });
	ufos_.push_back(std::move(ufo_));
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3Calc::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	//screws_.
	screws_.remove_if([](std::unique_ptr<Screw>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
		});
	bool isRelese = false;
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		(*block)->Update();
		isRelese = isRelese || (*block)->GetIsRelese();
	}
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		(*block)->Update();
		if ((*block)->GetIsDead()) {
			isRelese = isRelese || (*block)->GetIsRelese();
		}
	}

	//ブロックと死んだUFOを一つの一時リストにまとめる
	std::vector<Block*> blockUFO;
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		blockUFO.push_back((*block).get());
	}
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		if ((*block)->GetIsDead()) {
			blockUFO.push_back((*block).get());
		}
	}

	if (isRelese) {
		for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
			if (!(*block)->GetIsCenter()) {
				(*block)->SetIsConnect(false);
			}
		}
		collisionManager_->ListClear();
		int oldCount = 0;
		int newCount = 0;
		for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
			collisionManager_->ListRegister((*block)->GetCollider());
		}
		do {
			oldCount = newCount;
			newCount = 0;
			collisionManager_->CheakAllCollision();
			for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
				if ((*block)->GetIsConnect()) {
					newCount++;
				}
			}
		} while (oldCount != newCount);
	}

	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Update();
	}
	

	target_.Update(&blockUFO, *followCamera_.get(), player_.get());
	player_->Update(target_.GetTargetBlock(), target_.GetNumTargetAnchor());
	//ufo_->Update();
	/*Block* center = nullptr;
	//中心となるブロックをリセット
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		if ((*block)->GetIsCenter()) {
			center = (*block);
		}
	}*/


	collisionManager_->ListClear();
	collisionManager_->ListRegister(player_->GetCollider());
	collisionManager_->ListRegister(player_->GetMagnet()->GetCollider());
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		if (!(*block)->GetIsDead()) {
			collisionManager_->ListRegister((*block)->GetCollider());
			collisionManager_->ListRegister((*block)->GetAttract()->GetCollider());
		}
		else {
			Block* castBlock = (*block).get();
			collisionManager_->ListRegister(castBlock->GetCollider());
			collisionManager_->ListRegister((*block)->GetMagnet()->GetCollider());
		}
	}
	

	//collisionManager_->ListRegister(blocks_[0]->GetCollider());
	//collisionManager_->ListRegister(blocks_[1]->GetCollider());
	//collisionManager_->ListRegister(blocks_[2]->GetCollider());
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
	}
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
	}
	collisionManager_->CheakAllCollision();
	/*
	if (center) {
		for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
			if ((*block)->GetIsCenter()) {
				if (center != (*block).get()) {
					center->SetIsCenter(false);
				}
			}
		}
}*/
	// スカイドーム
	skydome_->Update();

	// 惑星
	planet_->Update();

	camera_.Update();

	followCamera_->Update();
	camera_ = static_cast<BaseCamera>(*followCamera_.get());

	// UIマネージャー
	uiManager_->Update();

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
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
	}
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Draw(modelScrew_.get(), camera_);
	}
	player_->Draw(modelBlock_.get(), camera_);
	//ufo_->Draw(modelBlock_.get(), camera_);
	

	// スカイドーム
	skydome_->Draw(camera_);

	// 惑星
	planet_->Draw(camera_);

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

	// UIマネージャー
	uiManager_->Draw();

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

	// スカイドーム
	skydome_->ImGuiDraw();

	// 惑星
	//planet_->ImGuiDraw();

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

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

	// 惑星
	planetModel_.reset(Model::Create("Resources/Planet/", "planet.obj", dxCommon_, textureHandleManager_.get()));

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

	uiTextureHandles_ = {
		TextureManager::Load("Resources/UI/kugimi.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/number.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/ingame_ui_symbol.png", dxCommon_,textureHandleManager_.get()),
	};

	shotUITextureHandle_[0] = TextureManager::Load("Resources/ingame_ui_RB.png", dxCommon_, textureHandleManager_.get());
	shotUITextureHandle_[1] = TextureManager::Load("Resources/ingame_ui_RB_remove.png", dxCommon_, textureHandleManager_.get());
}
