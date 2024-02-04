#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../Player/player.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../ForResult/ForResult.h"
#include "../../Particle/Liner/ForLinerEmitterData.h"

TutorialScene::~TutorialScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	IScene::Initialize();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Tuturial";

	globalVariables->AddItem(groupName, "FirstScrewNum", firstScrewNum_);
	firstScrewNum_ = globalVariables->GetIntValue(groupName, "FirstScrewNum");

	
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
	particleModel[ParticleModelIndex::kStar] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);

	isDebugCameraActive_ = false;

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	
	player_.reset(new Player);
	player_->Initialize(playerModels_);

	for (int index = 0; index < firstScrewNum_; index++) {
		std::unique_ptr<Screw> screw;
		screw.reset(new Screw);
		screw->Initialize();
		screw->SetPlayer(player_.get());
		screw->SetSweatTextureHandle(dropTextureHandle_);
		screws_.push_back(std::move(screw));
	}

	player_->SetScrew(&screws_);
	//player_->SetViewProjection(camera_);

	target_.Initialize(cursorTextureHandle_, shotUITextureHandle_, arrowTextureHandle_, lockonTextureHandle_, stickeTextureHandle_);
	followCamera_.reset(new FollowCamera);
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransform());
	player_->SetViewProjection(*followCamera_.get());

	player_->SetCircle(modelCircle_.get());

	//UIマネージャー
	//uiManager_ = std::make_unique<UIManager>();
	//uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();
	player_->SetAudioManager(audioManager_.get());
	Screw::StaticInitialize(audioManager_.get());
	//uiManager_->SetAudioManager(audioManager_.get());

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	blocks_.clear();
	std::unique_ptr<Block> block;
	block.reset(new Block);
	block->Initialize();
	//block->SetWorldPosition({ -20.0f,0.0f,20.0f });
	block->SetVelocity({ 0,0,0 });
	block->SetIsCenter(true);
	block->SetIsConnect(true);
	block->SetSize({4.0f,2.0f,4.0f});
	blocks_.push_back(std::move(block));
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({-20.0f,0.0f,40.0f});
	block->SetVelocity({0,0,0});
	blocks_.push_back(std::move(block));
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 20.0f,0.0f,40.0f });
	block->SetVelocity({ 0,0,0 });
	blocks_.push_back(std::move(block));
	block.reset(new Block);
	block->Initialize();
	block->SetWorldPosition({ 0.0f,0.0f,60.0f });
	block->SetVelocity({ 0,0,0 });
	blocks_.push_back(std::move(block));
	ufos_.clear();
	
	energy_.reset(new Energy);
	energy_->Initialize();
	energy_->SetTarget({ -100.0f,0.0f,-12.0f });
	energyPoint_ = energyMax_;
	outline_.Initialize();
	outline_.color_ = { 0.8f,0.4f,0.1f,1.0f };
	sweatModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	//影
	shadowManager_ = std::make_unique<ShadowManager>();
	shadowManager_->Initialize(shadowModel_.get());

	// クリア演出
	//clearMigration_ = std::make_unique<ClearMigration>();
	//clearMigration_->Initialize(clearMigrationTextureHandle_);

	// ポーズ
	pause_ = std::make_unique<Pause>();
	pause_->Initialize(pauseTextureHandles_);
	pause_->SetAudioManager(audioManager_.get());

	// ロケット
	rocket_ = std::make_unique<Rocket>();
	rocket_->Initialize(rocketModel_.get(), soilModel_.get());
	player_->SetRocket(rocket_.get());

	audioManager_->PlayWave(kGameAudioNameIndexBGM);

	ForLinerEmitterData::GetInstance()->SetIsDraw(false);
	TransformStructure transform{ {1.0f,1.0f,1.0f},{0},{0.0f,3.0f,0.0f} };
	ParticleManager::GetInstance()->MakeEmitter(transform, 300, 0.005f, 0.5f, ParticleModelIndex::kCircle, ParticleName::kLinerParticle, EmitterName::kLinerEmitter);

	player_->SetIsCanShot(false);
	player_->SetIsCanGravity(false);
	player_->SetIsCanLockOn(false);
	followCamera_->SetIsCanLockOn(false);
	target_.SetIsDraw(false);
	target_.SetIsCanLockOn(false);
	for (int i = 0; i < 7;i++) {
		isBeenMissionUpdate_[i] = false;
		isClearMission_[i] = 0;
	}

	tutorialUIManager = std::make_unique<TutorialUIManager>();
	tutorialUIManager->Initialize(tutorialUITextureHandles_);
	tutorialUIManager->SetAudioManager(audioManager_.get());

}

/// <summary>
/// 更新処理
/// </summary>
void TutorialScene::Update() {
	ImguiDraw();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerJoystick(JoystickButton::kJoystickButtonSTART)) {
		//requestSceneNo = kTitle;
	}
#endif
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_G)) {
		TransformStructure transform{ {1.0f,1.0f,1.0f},{0},{0.0f,3.0f,0.0f} };
		ParticleManager::GetInstance()->MakeEmitter(transform, 3, 0.005f, 0.5f, ParticleModelIndex::kCircle, ParticleName::kGravityParticle, EmitterName::kGravityEmitter);
	}
#endif

	if (requestSceneNo == kClear || requestSceneNo == kTitle || isBeingReset_) {
		pause_->SetRestart(false);
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// タイトルへ行く
	GoToTheTitle();

	// リスタート
	if (pause_->Restart()) {
		resetScene_ = true;
		isBeingReset_ = true;
		isDecreasingVolume = true;
	}

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

	screws_.remove_if([](std::unique_ptr<Screw>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
		});
	bool isRelese = false;
	int oldConnectCount = 0;
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
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
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		blockUFO.push_back((*block).get());
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
		if ((*block)->GetIsCenter()) {
			collisionManager_->ListRegister((*block)->GetCollider());
		}
		else if ((*block)->GetIsConnect() && (*block)->GetAnchorPointScrew(0)) {
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

	target_.Update(&blockUFO, *followCamera_.get(), player_.get(), &screws_);
	if (target_.IsTarget() && target_.IsLockedChange()) {
		player_->SetTarget(target_.GetTargetBlock()->GetWorldTransform());
	}
	else {
		player_->SetTarget(nullptr);
	}

	int followCount = 0;
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->SetIsFirstStuck(false);
		if ((*block)->GetState() == Screw::FOLLOW) {
			followCount++;
		}
	}
	ForLinerEmitterData::GetInstance()->SetIsDraw(false);
	if (followCount == 0) {
		Screw* screw = nullptr;
		int length = -1;
		for (std::list<std::unique_ptr<Screw>>::iterator ite = screws_.begin(); ite != screws_.end(); ite++) {
			if ((*ite)->GetState() == Screw::STUCK && (*ite)->GetStackLength() > length) {
				length = (*ite)->GetStackLength();
				screw = (*ite).get();
			}
		}
		ForLinerEmitterData::GetInstance()->SetIsDraw(false);
		if (screw && player_->GetBehavior() == Player::Behavior::kRoot) {
			screw->SetIsFirstStuck(true);
			std::array<WorldTransform, PlayerPartIndex::kPlayerPartIndexOfCount>* transforms = player_->GetAnimation()->GetWorldTransforms();
			Vector3 end = (*transforms)[PlayerPartIndex::kPlayerPartIndexMagnet].GetWorldPosition();
			Vector3 start = screw->GetWorldTransform()->GetWorldPosition();
			start.y += 2.0f;
			ForLinerEmitterData::GetInstance()->SetData(start, end);
			ForLinerEmitterData::GetInstance()->SetIsDraw(true);
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
	if (player_->GetIsRideConnectFlooar()) {
		collisionManager_->ListRegister(player_->GetMagnet()->GetCollider());
	}
	player_->SetIsRideConnectFlooar(false);
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
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
	}
	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		collisionManager_->ListRegister((*block)->GetCollider());
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
	//missionData_->Update(connectCount, player_->GetWorldTransform()->GetWorldPosition());
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

	//各項目クリア判定
	for (int i = 0; i < 7; i++) {
		isBeenMissionUpdate_[i] = false;
	}
	//移動してみよう
	if (!isClearMission_[0] && player_->GetControlLength()>120) {
		isClearMission_[0] = true;
		isBeenMissionUpdate_[0] = true;
	}
	//カメラを動かしてみよう
	if (!isClearMission_[1] && followCamera_->GetControlLength() > 30) {
		isClearMission_[1] = true;
		isBeenMissionUpdate_[1] = true;
	}
	if (isClearMission_[0] && isClearMission_[1] && !isClearMission_[3]) {
		player_->SetIsCanShot(true);
		target_.SetIsDraw(true);
	}

	//ネジを打ってみよう
	if (!isClearMission_[2]) {
		bool isStuck = false;
		for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
			if ((*block)->GetAnchorPointScrew(0)) {
				isStuck = true;
				break;
			}
		}
		if (isStuck) {
			isClearMission_[2] = true;
			isBeenMissionUpdate_[2] = true;
		}
	}
	//ブロックに近づいてみよう
	if (!isClearMission_[3]) {
		bool isConnect = false;
		for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
			isConnect = isConnect || ((*block)->GetIsConnect() && !(*block)->GetIsCenter());
		}
		if (isConnect) {
			isClearMission_[3] = true;
			isBeenMissionUpdate_[3] = true;
			std::unique_ptr<Screw> screw;
			screw.reset(new Screw);
			screw->Initialize();
			screw->SetPlayer(player_.get());
			screw->SetSweatTextureHandle(dropTextureHandle_);
			screws_.push_back(std::move(screw));
		}
	}
	if (isClearMission_[3]) {
		target_.SetIsCanLockOn(true);
		player_->SetIsCanLockOn(true);
		followCamera_->SetIsCanLockOn(true);
	}
	//一時的に撃つのを再封印
	if (isClearMission_[3] && !isClearMission_[4]) {
		player_->SetIsCanShot(false);
	}

	//ロックオンしてみよう
	if (!isClearMission_[4] && target_.IsLockedChange()) {
		isClearMission_[4] = true;
		isBeenMissionUpdate_[4] = true;
		//player_->SetIsCanShot(true);
	}
	if (isClearMission_[4] && target_.GetIsChangeTargetBlock()){
		player_->SetIsCanShot(true);
	}
	//照準を切り替えて
	if (!isClearMission_[5] && target_.GetIsChangeTargetBlock()) {
		isClearMission_[5] = true;
		isBeenMissionUpdate_[5] = true;
		player_->SetIsCanGravity(true);
	}
	if (!isClearMission_[6]) {
		bool isReStuck = false;
		for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
			isReStuck = isReStuck || (*block)->GetIsReStuck();
		}
		if (isReStuck) {
			isClearMission_[6] = true;
			isBeenMissionUpdate_[6] = true;
			//kari
			requestSceneNo = kGame;
		}
	}


	// 影
	ShadowUpdate();

	// スカイドーム
	skydome_->Update();

	// ロケット
	rocket_->Update();

	

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
	/*
	UIManagerUpdateDesc uiManagerUpdateDesc = {
		gameTimer_,
		screwCount,
		missionData_->GetMissionBlockVector()[MissionData::GetInstance()->GetMissionNumBlock()].num,
		connectCount,
		missionData_->IsMissionBlockBeenUpdate(),
		missionData_->GetMissionNumPoint(),
		missionData_->IsMissionPointBeenUpdate(),
		missionData_->IsCompleteBlock(),
		missionData_->IsCompletePoint()
	};
	uiManager_->Update(uiManagerUpdateDesc);
	*/

	// UI 
	tutorialUIManager->Update(isBeenMissionUpdate_, isClearMission_);

	// デバッグカメラ
	DebugCameraUpdate();

	//パーティクル
	particleManager_->Update(camera_);

	//アウトライン
	outline_.Map();

	// クリア演出
	/*clearMigration_->Update();
	if (clearMigration_->GetIsEnd()) {
		requestSceneNo = kClear;
		isDecreasingVolume = true;
		ForResult::GetInstance()->connectNum_ = connectCount;
	}

	gameTimerFloat_ -= kDeltaTime_;
	gameTimer_ = int(gameTimerFloat_);
	if (gameTimerFloat_ - float(gameTimer_) > 0) {
		gameTimer_++;
	}
	if (gameTimer_ < 0) {
		gameTimer_ = 0;
		if (requestSceneNo != kClear) {
			clearMigration_->Stert();
		}
	}
	*/
#ifdef _DEBUG

	ImGui::Begin("TIMER");
	ImGui::Text("%d", gameTimer_);
	ImGui::End();

#endif // _DEBUG
}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {

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

	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
		(*block)->DrawSoil(soilModel_.get(), camera_);
	}
	for (std::vector<std::unique_ptr<UFO>>::iterator block = ufos_.begin(); block != ufos_.end(); block++) {
		(*block)->Draw(modelBlock_.get(), camera_);
	}
	Matrix4x4 billBoardMatrix = Matrix4x4Calc::MakeIdentity4x4();
	billBoardMatrix = Matrix4x4Calc::Multiply(Matrix4x4Calc::MakeRotateXYZMatrix({ 0.0f,3.141592f,0.0f }), Matrix4x4Calc::Inverse(camera_.GetViewMatrix()));
	billBoardMatrix.m[3][0] = 0;
	billBoardMatrix.m[3][1] = 0;
	billBoardMatrix.m[3][2] = 0;

	for (std::list<std::unique_ptr<Screw>>::iterator block = screws_.begin(); block != screws_.end(); block++) {
		(*block)->Draw(modelScrew_.get(), camera_);
		if ((*block)->GetIsDrawSweat()) {
			(*block)->DrawSweat(sweatModel_.get(), camera_, billBoardMatrix);
		}
	}

	//ufo_->Draw(modelBlock_.get(), camera_);
	//energy_->Draw(modelBlock_.get(), camera_);


	rocket_->Draw(camera_);

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
		(*block)->DrawOutLine(modelScrew_.get(), camera_, outline_);
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
	//uiManager_->Draw();
	tutorialUIManager->Draw();

	target_.SpriteDraw();

	//clearMigration_->Draw();

	pause_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TutorialScene::ImguiDraw() {
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

void TutorialScene::DebugCameraUpdate()
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

void TutorialScene::GoToTheTitle()
{

	if (pause_->GoToTheTitle()) {
		requestSceneNo = kTitle;
		isDecreasingVolume = true;
	}

}

void TutorialScene::ModelCreate()
{

	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

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

	// ロケット
	rocketModel_.reset(Model::Create("Resources/Model/rocket/", "rocket.obj", dxCommon_, textureHandleManager_.get()));

}

void TutorialScene::MaterialCreate()
{

}

void TutorialScene::TextureLoad()
{

	cursorTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/UI/ingame_target.png", dxCommon_, textureHandleManager_.get());
	/*
	uiTextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Game/UI/nejimit.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/number.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_symbol.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/mission_number.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/missionClear.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_destination_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_destinationColor_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/missionComplete.png", dxCommon_,textureHandleManager_.get()),


		TextureManager::Load("Resources/Sprite/Common/ingame_ui_LB.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_LB_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/controler_UI_A.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_A_text.png", dxCommon_,textureHandleManager_.get()),
	};
	*/
	shotUITextureHandle_[0] = TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_RB.png", dxCommon_, textureHandleManager_.get());
	shotUITextureHandle_[1] = TextureManager::Load("Resources/Sprite/Game/UI/ingame_ui_RB_remove.png", dxCommon_, textureHandleManager_.get());

	whiteTextureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon_, textureHandleManager_.get());

	//clearMigrationTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/ingame_fnish.png", dxCommon_, textureHandleManager_.get());

	pauseTextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_flame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_ingame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_restart.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_text_title.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_arrow.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/Pause/pause_choiceBox.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/default/white2x2.png", dxCommon_,textureHandleManager_.get()),
	};
	dropTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/drops.png", dxCommon_, textureHandleManager_.get());
	arrowTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/Pause/pause_arrow.png", dxCommon_, textureHandleManager_.get());
	lockonTextureHandle_ = TextureManager::Load("Resources/Sprite/Game/UI/ingame_rockon_UI.png", dxCommon_, textureHandleManager_.get());

	stickeTextureHandle_[0] = TextureManager::Load("Resources/Sprite/Game/UI/controler_UI_rightStic_left.png", dxCommon_, textureHandleManager_.get());
	stickeTextureHandle_[1] = TextureManager::Load("Resources/Sprite/Game/UI/controler_UI_rightStic_right.png", dxCommon_, textureHandleManager_.get());

	tutorialUITextureHandles_ = {
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/ingame_mission_frame.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Game/UI/missionClear.png", dxCommon_,textureHandleManager_.get()),
		
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_move_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_cameraMove_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_screwShot_text.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_screwBlock_text.png", dxCommon_,textureHandleManager_.get()), 
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_rockon_text.png", dxCommon_,textureHandleManager_.get()), 
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_rockonChange_text.png", dxCommon_,textureHandleManager_.get()), 
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_screwDown_text.png", dxCommon_,textureHandleManager_.get()), 
		
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_skip_text.png", dxCommon_,textureHandleManager_.get()), 
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_skipGage.png", dxCommon_,textureHandleManager_.get()), 
		TextureManager::Load("Resources/Sprite/Tutorial/tuterial_skipGageFrame.png", dxCommon_,textureHandleManager_.get()), 
		
		TextureManager::Load("Resources/Sprite/Common/controler_UI_leftStic1.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/controler_UI_rightStic1.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/ingame_ui_LB.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/ingame_ui_RB.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/controler_UI_A.png", dxCommon_,textureHandleManager_.get()),
		TextureManager::Load("Resources/Sprite/Common/controler_UI_Y.png", dxCommon_,textureHandleManager_.get()),
	};

}

void TutorialScene::LowerVolumeBGM()
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

void TutorialScene::ShadowUpdate()
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
	for (std::vector<std::unique_ptr<Block>>::iterator block = blocks_.begin(); block != blocks_.end(); block++) {
		magnification = { 4.0f, 0.5f, 4.0f };
		size = {
		(*block)->GetWorldTransform()->transform_.scale.x * magnification.x,
		(*block)->GetWorldTransform()->transform_.scale.y * magnification.y,
		(*block)->GetWorldTransform()->transform_.scale.z * magnification.z };
		shadowManager_->ShadowAppearsObjListRegister((*block)->GetWorldTransform()->GetWorldPosition(), size);
	}


	// 影が出るか
	shadowManager_->SeeShadow();

}
