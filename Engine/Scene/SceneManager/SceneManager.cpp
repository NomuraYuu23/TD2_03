#include "SceneManager.h"
#include "../../base/TextureManager.h"

void SceneManager::Initialize()
{

	// シーンファクトリー
	sceneFacyory_ = SceneFactory::GetInstance();

	// シーン(タイトル)
	scene_.reset(sceneFacyory_->CreateScene(kTitle));
	scene_->Initialize();

	// 初期シーン
	currentSceneNo_ = kTitle;

	requestSeneNo_ = kTitle; // リクエストシーン
	prevRequestSeneNo_ = kTitle; // 前のリクエストシーン

	// シーンの静的初期化
	scene_->StaticInitialize();

	// シーン遷移ファクトリー
	sceneTransitionFactory_ = SceneTransitionFactory::GetInstance();

	// シーン遷移を保持するメンバ変数
	sceneTransition_ = nullptr;

}

void SceneManager::Update()
{

	// シーンのチェック
	currentSceneNo_ = scene_->GetSceneNo();

	prevRequestSeneNo_ = requestSeneNo_; // 前のリクエストシーン
	requestSeneNo_ = scene_->GetRequestSceneNo(); // リクエストシーン

	// シーン変更チェック
	//if (prevSceneNo_ != currentSceneNo_) {
	//	TextureManager::GetInstance()->ResetTexture();
	//	scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
	//	scene_->Initialize();
	//}

	// リクエストシーンが変わったか
	if (requestSeneNo_ != prevRequestSeneNo_) {
		//シーン遷移開始（初期化）
		sceneTransition_.reset(sceneTransitionFactory_->CreateSceneTransition(currentSceneNo_, requestSeneNo_));
		sceneTransition_->Initialize();
	}

	//シーン遷移中
	if (sceneTransition_) {
		// シーン遷移更新
		sceneTransition_->Update();
		if (sceneTransition_->GetSwitchScene()) {
			// シーン切り替え
			TextureManager::GetInstance()->ResetTexture();
			currentSceneNo_ = requestSeneNo_;
			scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
			scene_->Initialize();
			sceneTransition_->SetSwitchScene(false);
		}
		else if (!sceneTransition_->GetTransitioning()) {
			sceneTransition_.release();
			sceneTransition_ = nullptr;
		}
	}

	// 更新処理
	scene_->Update();

}

void SceneManager::Draw()
{
	// 描画処理
	scene_->Draw();
	if (sceneTransition_) {
		sceneTransition_->Draw();
	}

}
