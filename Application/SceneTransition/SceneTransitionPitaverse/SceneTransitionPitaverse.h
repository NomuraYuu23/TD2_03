#pragma once
#include "../../../Engine/SceneTransition/ISceneTransition/ISceneTransition.h"
#include <memory>
#include "../../../Engine/2D/Sprite.h"

class SceneTransitionPitaverse : public ISceneTransition
{

public: // メンバ関数

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

private:

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

private:

	// 背景
	std::unique_ptr<Sprite> backgroundSprite_;
	uint32_t backgroundTextureHandle_;
	Vector4 backgroundColor_;

	// ロード文字
	std::unique_ptr<Sprite> loadSprite_;
	uint32_t loadTextureHandle_;
	Vector4 loadColor_;

	uint32_t loadCount_;

};

