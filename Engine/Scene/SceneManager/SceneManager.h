#pragma once

#include "../IScene/IScene.h"
#include "../TitleScene/TitleScene.h"
#include "../GameScene/GameScene.h"
#include "../SceneFactory/SceneFactory.h"
#include "../../SceneTransition/ISceneTransition/ISceneTransition.h"
#include "../../SceneTransition/SceneTransitionFactory/SceneTransitionFactory.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数

	// シーンファクトリー	
	SceneFactory* sceneFacyory_ = nullptr;

	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン

	int requestSeneNo_; // リクエストシーン
	int prevRequestSeneNo_; // 前のリクエストシーン

	// シーン遷移ファクトリー
	SceneTransitionFactory* sceneTransitionFactory_ = nullptr;

	// シーン遷移を保持するメンバ変数
	std::unique_ptr<ISceneTransition> sceneTransition_;

};

