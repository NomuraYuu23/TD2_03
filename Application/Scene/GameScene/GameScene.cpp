#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../Player/player.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../ForResult/ForResult.h"
/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Screw";

	globalVariables->AddItem(groupName, "FirstScrewNum", firstScrewNum_);
	firstScrewNum_ = globalVariables->GetIntValue(groupName, "FirstScrewNum");
	
	const std::string groupName2 = "Timer";
	globalVariables->AddItem(groupName2, "MAX", timerMax_);
	timerMax_ = globalVariables->GetIntValue(groupName2, "MAX");
	gameTimer_ = timerMax_;
	gameTimerFloat_ = float(gameTimer_);
	ModelCreate();
	MaterialCreate();
	TextureLoad();

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

	blockManager_ = std::make_unique<BlockManager>();
	blockManager_->Initialize(modelBlock_.get());

	player_.reset(new Player);
	player_->Initialize(playerModels_);

	for (int index = 0; index < firstScrewNum_; index++) {
		std::unique_ptr<Screw> screw;
		screw.reset(new Screw);
		screw->Initialize();
		screw->SetPlayer(player_.get());
		screw->SetSweatTextureHandle(TextureManager::Load("Resources/Sprite/Game/drops.png", dxCommon_, textureHandleManager_.get()));
		screws_.push_back(std::move(screw));
	}

	player_->SetScrew(&screws_);
	//player_->SetViewProjection(camera_);

	target_.Initialize(cursorTextureHandle_,shotUITextureHandle_);
	followCamera_.reset(new FollowCamera);
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetViewProjection(*followCamera_.get());

	player_->SetCircle(modelCircle_.get());

	blockManager_->SetPlayer(player_.get());

	//UIマネージャー
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();
	player_->SetAudioManager(audioManager_.get());
	Screw::StaticInitialize(audioManager_.get());
	uiManager_->SetAudioManager(audioManager_.get());

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 惑星
	//planet_ = std::make_unique<Planet>();
	//planet_->Initialize(planetModel_.get());

	/*
	std::unique_ptr<UFO> ufo_;
	ufo_.reset(new UFO);
	ufo_->Initialize();
	ufo_->SetCircle(modelCircle_.get());
	ufo_->SetWorldPosition({ -10.0f,12.0f,150.0f });
	ufo_->SetVelocity({ 0.0f,0.0f,-0.1f });
	ufos_.push_back(std::move(ufo_));
	*/
	ufos_.clear();
	ufoManager_.reset(new UFOManager);
	ufoManager_->Initialize();
	ufoManager_->SetUFOVector(&ufos_);
	ufoManager_->SetModelCircle(modelCircle_.get());

	energy_.reset(new Energy);
	energy_->Initialize();
	energy_->SetTarget({ -100.0f,0.0f,-12.0f });
	energyPoint_ = energyMax_;
	outline_.Initialize();
	outline_.color_ = { 0.8f,0.4f,0.1f,1.0f };
	missionNum_ = 0;
	mission_.clear();
	mission_.push_back(2);
	mission_.push_back(6);
	mission_.push_back(10);
	mission_.push_back(14);
	mission_.push_back(20);
	mission_.push_back(24);
	mission_.push_back(28);
	mission_.push_back(32);
	mission_.push_back(36);
	mission_.push_back(40);
	missionData_ = MissionData::GetInstance();
	missionData_->Initialize();
	//MissionData::GetInstance()->SetMax(mission_.size());
	std::vector<MissionData::MissionToPoint>& point = MissionData::GetInstance()->GetMissionPointVector();
	for (size_t num = 0; num < point.size();num++) {
		std::unique_ptr<Planet> planet;
		planet.reset(new Planet);
		planet->Initialize(planetModel_.get());
		planet->SetFlagModel(planetFlagModel_.get());
		planet->SetPosition(point[num].point);
		planets_.push_back(std::move(planet));
	}

	sweatModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	//影
	shadowManager_ = std::make_unique<ShadowManager>();
	shadowManager_->Initialize(shadowModel_.get());

	// クリア演出
	clearMigration_ = std::make_unique<ClearMigration>();
	clearMigration_->Initialize(clearMigrationTextureHandle_);

	// ポーズ
	pause_ = std::make_unique<Pause>();
	pause_->Initialize(pauseTextureHandles_);
	pause_->SetAudioManager(audioManager_.get());

	audioManager_->PlayWave(kGameAudioNameIndexBGM);

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerJoystick(JoystickButton::kJoystickButtonSTART)) {
		//requestSceneNo = kTitle;
	}
#endif
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_G)) {
		TransformStructure transform{ {1.0f,1.0f,1.0f},{0},{0.0f,3.0f,0.0f} };
		ParticleManager::GetInstance()->MakeEmitter(transform,3,0.005f,0.5f, ParticleModelIndex::kCircle,ParticleName::kGravityParticle,EmitterName::kGravityEmitter);
	}
#endif

	// BGM音量下げる
	if (requestSceneNo == kClear || requestSceneNo == kTitle) {
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// タイトルへ行く
	GoToTheTitle();

	// ポーズ機能
	pause_->PoseSwitching();
	if (pause_->IsPause()) {
		pause_->Update();
		return;
	}

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3Calc::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	blockManager_->Update();

	std::vector<MissionData::MissionToPoint>& point = MissionData::GetInstance()->GetMissionPointVector();
	for (size_t num = 0; num < point.size(); num++) {
		planets_[num]->SetPosition(point[num].point);
		
	}

	//screws_.
	screws_.remove_if([](std::unique_ptr<Screw>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
		});
	bool isRelese = false;
	int oldConnectCount = 0;
	for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
		(*block)->Update();
		(*block)->SetWhiteTextureHandle(whiteTextureHandle_);
		isRelese = isRelese || (*block)->GetIsRelese();
		if ((*block)->GetIsConnect() && !(*block)->GetIsCenter()) {
			oldConnectCount++;
		}
	}
	//ufoManager_->Update();
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		(*block)->Update();
		if ((*block)->GetIsDead()) {
			(*block)->SetWhiteTextureHandle(whiteTextureHandle_);
			isRelese = isRelese || (*block)->GetIsRelese();
			if ((*block)->GetIsConnect() && !(*block)->GetIsCenter()) {
				oldConnectCount++;
			}
		}
	}

	//ブロックと死んだUFOを一つの一時リストにまとめる
	std::vector<Block*> blockUFO;
	for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
		blockUFO.push_back(*block);
	}
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		if ((*block)->GetIsDead()) {
			blockUFO.push_back((*block).get());
		}
	}

	if (isRelese) {
		for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
			if (!(*block)->GetIsCenter()) {
				(*block)->SetReConnect(true);
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
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
			(*block)->SetReConnect(false);
	}
	//外れかかっているかの確認
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		if (!(*block)->GetIsCenter()) {
			(*block)->SetReConnect(true);
			//(*block)->SetIsConnect(false);
			(*block)->SetPinchCheckMode(true);
		}
	}
	collisionManager_->ListClear();
	int oldCount = 0;
	int newCount = 0;
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		if ((*block)->GetIsCenter() ) {
				collisionManager_->ListRegister((*block)->GetCollider());
		}
		else if ((*block)->GetIsConnect() && (*block)->GetAnchorPointScrew(0) ) {
			if (!(*block)->GetAnchorPointScrew(0)->GetIsPinch() && !(*block)->GetAnchorPointScrew(0)->GetIsFirstStuck()) {
				//(*block)->SetIsConnect(false);
				collisionManager_->ListRegister((*block)->GetCollider());
			}
		}
	}
	do {
		oldCount = newCount;
		newCount = 0;
		collisionManager_->CheakAllCollision();
		for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
			if (!(*block)->IsPinch()) {
				newCount++;
			}
		}
	} while (oldCount != newCount);
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		(*block)->SetPinchCheckMode(false);
		(*block)->SetReConnect(false);
	}
	collisionManager_->ListClear();

	target_.Update(&blockUFO, *followCamera_.get(), player_.get(),&screws_);
	if (target_.IsTarget() && target_.IsLockedChange()) {
		player_->SetTarget(target_.GetTargetBlock()->GetWorldTransform());
	}
	else {
		player_->SetTarget(nullptr);
	}

	int followCount=0;
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->SetIsFirstStuck(false);
		if ((*block)->GetState() == Screw::FOLLOW){
			followCount++;
		}
	}
	if (followCount == 0) {
		Screw* screw = nullptr;
		int length = -1;
		for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_.begin(); ite != screws_.end(); ite++) {
			if ((*ite)->GetState() == Screw::STUCK && (*ite)->GetStackLength() > length) {
				length = (*ite)->GetStackLength();
				screw = (*ite).get();
			}
		}
		if (screw) {
			screw->SetIsFirstStuck(true);
		}
	}
	player_->Update(target_.GetTargetBlock(), target_.GetNumTargetAnchor());
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Update();
	}

	//ufo_->Update();
	/*Block* center = nullptr;
	//中心となるブロックをリセット
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		if ((*block)->GetIsRidePlayer()) {
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
	collisionManager_->ListRegister(energy_->GetCollider());

	//collisionManager_->ListRegister(blocks_[0]->GetCollider());
	//collisionManager_->ListRegister(blocks_[1]->GetCollider());
	//collisionManager_->ListRegister(blocks_[2]->GetCollider());
	for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
	}
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
	}
	for (std::vector<std::unique_ptr<Planet>>::iterator ite = planets_.begin(); ite != planets_.end(); ite++) {
		collisionManager_->ListRegister((*ite)->GetCollider());
	}
	collisionManager_->CheakAllCollision();
	
	/*if (center) {
		for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
			if ((*block)->GetIsCenter()) {
				if (center != (*block)) {
					center->SetIsRidePlayer(false);
				}
			}
		}
	}*/

	//接続数カウント
	int connectCount = 0;
	for (std::vector<Block*>::iterator block = blockUFO.begin(); block != blockUFO.end(); block++) {
		if ((*block)->GetIsConnect() && !(*block)->GetIsCenter()) {
			connectCount++;
		}
	}
	if (oldConnectCount < connectCount) {
		followCamera_->Shake();
		// 合体
		audioManager_->PlayWave(kGameAudioNameIndexUnion1);
	}
	// このフレームでミッションが更新されたか
	missionData_->Update(connectCount,player_->GetWorldTransform()->GetWorldPosition());
#ifdef _DEBUG

	ImGui::Begin("MISSION");
	ImGui::Text("%d", mission_[missionNum_]);
	ImGui::End();
	
#endif // _DEBUG
	//エネルギー増減仮
	/* {
		energyPoint_ -= connectCount * 0.1f;
		energyPoint_ += energy_->GetInnerAreaCount() * 0.4f;
		if (energyPoint_ > energyMax_) {
			energyPoint_ = energyMax_;
		}
		if (energyPoint_ < 0.0f) {
			energyPoint_ = 0.0f;
		}
		ImGui::Begin("Energy");
		ImGui::Text("%f", energyPoint_);
		ImGui::End();
	}
	energy_->Update();
	*/

	// 影
	ShadowUpdate();

	// スカイドーム
	skydome_->Update();

	// 惑星
	//planet_->Update();
	for (std::vector<std::unique_ptr<Planet>>::iterator ite = planets_.begin(); ite != planets_.end();ite++) {
		(*ite)->Update();
	}

	camera_.Update();

	followCamera_->Update();
	camera_ = static_cast<BaseCamera>(*followCamera_.get());

	// UIマネージャー
	uint32_t screwCount = 0u;
	for (std::list<std::unique_ptr<Screw>>::iterator screw = screws_.begin(); screw != screws_.end(); screw++) {
		if (screw->get()->GetState() == Screw::FOLLOW) {
			screwCount++;
		}
	}

	UIManagerUpdateDesc uiManagerUpdateDesc = {
		gameTimer_,
		screwCount,
		missionData_->GetMissionBlockVector()[MissionData::GetInstance()->GetMissionNumBlock()],
		connectCount,
		missionData_->IsMissionBlockBeenUpdate(),
		missionData_->GetMissionNumPoint(),
		missionData_->IsMissionPointBeenUpdate(),
		missionData_->IsCompleteBlock(),
		missionData_->IsCompletePoint()
	};
	uiManager_->Update(uiManagerUpdateDesc);

	// デバッグカメラ
	DebugCameraUpdate();

	//パーティクル
	particleManager_->Update(camera_);

	//アウトライン
	outline_.Map();

	// クリア演出
	clearMigration_->Update();
	if (clearMigration_->GetIsEnd()) {
		requestSceneNo = kClear;
		isDecreasingVolume = true;
		ForResult::GetInstance()->connectNum_ = connectCount;
	}

	gameTimerFloat_ -= kDeltaTime_;
	gameTimer_ = int(gameTimerFloat_);
	if (gameTimerFloat_ - float(gameTimer_)>0) {
		gameTimer_++;
	}
	if (gameTimer_< 0) {
		gameTimer_ = 0;
		if (requestSceneNo != kClear) {
			clearMigration_->Stert();
		}
	}

#ifdef _DEBUG

	ImGui::Begin("TIMER");
	ImGui::Text("%d", gameTimer_);
	ImGui::End();

#endif // _DEBUG
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
	
	// スカイドーム
	skydome_->Draw(camera_);

	for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
		(*block)->DrawSoil(soilModel_.get(), camera_);
	}
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
	}
	Matrix4x4 billBoardMatrix = Matrix4x4Calc::MakeIdentity4x4();
	billBoardMatrix = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeRotateXYZMatrix({ 0.0f,3.141592f,0.0f }) , Matrix4x4Calc::Inverse(camera_.GetViewMatrix()));
	billBoardMatrix.m[3][0] = 0;
	billBoardMatrix.m[3][1] = 0;
	billBoardMatrix.m[3][2] = 0;
	
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Draw(modelScrew_.get(), camera_);
		if ((*block)->GetIsDrawSweat()) {
			(*block)->DrawSweat(sweatModel_.get(), camera_,billBoardMatrix);
		}
	}

	//ufo_->Draw(modelBlock_.get(), camera_);
	//energy_->Draw(modelBlock_.get(), camera_);

	// 惑星
	//planet_->Draw(camera_);
	for (std::vector<std::unique_ptr<Planet>>::iterator ite = planets_.begin(); ite != planets_.end(); ite++) {
		(*ite)->Draw(camera_);
	}
	if (!MissionData::GetInstance()->GetMissionPointVector()[MissionData::GetInstance()->GetMissionNumPoint()].isClear_) {
		planets_[MissionData::GetInstance()->GetMissionNumPoint()]->DrawFlag(camera_);
	}

	shadowManager_->Draw(camera_);

	player_->Draw(modelPlayer_.get(), camera_);


#ifdef _DEBUG

	// デバッグ描画
	//colliderDebugDraw_->Draw(camera_);

#endif // _DEBUG

	Model::PostDraw();
	
#pragma region アウトライン描画
	Model::PreDrawOutLine(dxCommon_->GetCommadList());
	
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->DrawOutLine(modelScrew_.get(), camera_,outline_);
	}
	
	Model::PostDraw();

#pragma endregion

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

	// UIマネージャー
	uiManager_->Draw();

	target_.SpriteDraw();

	clearMigration_->Draw();

	pause_->Draw();

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
		requestSceneNo = kTitle;
		isDecreasingVolume = true;
	}

}

void GameScene::ModelCreate()
{

	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

	// 惑星
	planetModel_.reset(Model::Create("Resources/Model/target_planet", "target_planet.obj", dxCommon_, textureHandleManager_.get()));
	planetFlagModel_.reset(Model::Create("Resources/Model/target_flag", "flag.obj", dxCommon_, textureHandleManager_.get()));
	// プレイヤー
	playerModels_[kPlayerPartIndexBody].reset(Model::Create("Resources/Model/Player/Body", "playerBody.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexLeftLeg].reset(Model::Create("Resources/Model/Player/LeftLeg/", "playerLeftLeg.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexRightLeg].reset(Model::Create("Resources/Model/Player/RightLeg/", "playerRightLeg.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartIndexMagnet].reset(Model::Create("Resources/Model/Player/Magnet/", "playerMagnet.obj", dxCommon_, textureHandleManager_.get()));

	// 影
	shadowModel_.reset(Model::Create("Resources/Model/shadow/", "shadow.obj", dxCommon_, textureHandleManager_.get()));

	// 土
	soilModel_.reset(Model::Create("Resources/Model/soil_model/", "soil.obj", dxCommon_, textureHandleManager_.get()));

	// ねじ
	modelScrew_.reset(Model::Create("Resources/Model/nejimi2_model/", "nejimi2.obj", dxCommon_, textureHandleManager_.get()));

	// 円
	modelCircle_.reset(Model::Create("Resources/Model/Circle/", "Circle.obj", dxCommon_, textureHandleManager_.get()));

	// ブロック
	modelBlock_.reset(Model::Create("Resources/Model/Block/", "block.obj", dxCommon_, textureHandleManager_.get()));

}

void GameScene::MaterialCreate()
{

}

void GameScene::TextureLoad()
{

	cursorTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/UI/ingame_target.png", dxCommon_, textureHandleManager_.get());

	uiTextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Game/UI/kugimi.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/number.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_symbol.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/mission_number.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/missionClear.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_destination_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_destinationColor_text.png", dxCommon_,textureHandleManager_.get()),
	};

	shotUITextureHandle_[0] = TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_RB.png", dxCommon_, textureHandleManager_.get());
	shotUITextureHandle_[1] = TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_RB_remove.png", dxCommon_, textureHandleManager_.get());

	whiteTextureHandle_= TextureManager::Load("Resources/default/white2x2.png", dxCommon_, textureHandleManager_.get());

	clearMigrationTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/ingame_fnish.png", dxCommon_, textureHandleManager_.get());

	pauseTextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_flame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_ingame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_restart.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_title.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_arrow.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_choiceBox.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/default/white2x2.png", dxCommon_,textureHandleManager_.get()),
	};

}

void GameScene::LowerVolumeBGM()
{


	const uint32_t startHandleIndex = 3;

	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kGameAudioNameIndexBGM + startHandleIndex) {
			float decreasingVolume = 1.0f / 60.0f;
			float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
			if (volume < 0.0f) {
				volume = 0.0f;
				audioManager_->StopWave(i);
				isDecreasingVolume = false;
			}
			else {
				audioManager_->SetPlayingSoundDataVolume(i, volume);
				audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
			}
		}
	}

}

void GameScene::ShadowUpdate()
{

	// リストクリア
	shadowManager_->ListClear();
	
	// サイズ倍率
	Vector3 magnification = { 1.0f,1.0f,1.0f };
	// サイズ
	Vector3 size = { 1.0f,1.0f,1.0f };

	// リスト登録（影を発生させる物）

	// プレイヤー
	magnification = { 2.0f, 3.0f, 2.0f };
	size = {
	player_->GetWorldTransform()->transform_.scale.x * magnification.x,
	player_->GetWorldTransform()->transform_.scale.y * magnification.y,
	player_->GetWorldTransform()->transform_.scale.z * magnification.z };
	shadowManager_->CastsShadowObjListRegister(player_->GetWorldTransform()->GetWorldPosition(), size);

	// ねじ
	size = { 1.2f,1.2f,1.2f };
	for (std::list<std::unique_ptr<Screw>>::iterator screw = screws_.begin(); screw != screws_.end(); screw++) {
		shadowManager_->CastsShadowObjListRegister((*screw)->GetWorldTransform()->GetWorldPosition(), size);
	}

	// リスト登録（影が現れる物）
	// ブロック
	for (std::list<Block*>::iterator block = blockManager_->GetBlocks().begin(); block != blockManager_->GetBlocks().end(); block++) {
		magnification = { 4.0f, 0.5f, 4.0f };
		size = {
		(*block)->GetWorldTransform()->transform_.scale.x * magnification.x,
		(*block)->GetWorldTransform()->transform_.scale.y * magnification.y,
		(*block)->GetWorldTransform()->transform_.scale.z * magnification.z };
		shadowManager_->ShadowAppearsObjListRegister((*block)->GetWorldTransform()->GetWorldPosition(), size);
	}

	// プラネット
	for (std::vector<std::unique_ptr<Planet>>::iterator ite = planets_.begin(); ite != planets_.end(); ite++) {
		size = { 8.0f, 8.0f, 8.0f };
		shadowManager_->ShadowAppearsObjListRegister((*ite)->GetWorldTransform()->GetWorldPosition(), size);
	}

	// 影が出るか
	shadowManager_->SeeShadow();

}
