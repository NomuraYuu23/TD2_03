#pragma once
#include <memory>
#include "../../Engine/2D/Sprite.h"

class ClearMigration
{

public:

	void Initialize(uint32_t textureHandle);

	void Stert();

	void Update();

	void Draw();

	bool GetIsEnd() { return isEnd_; }

private:

	// スプライト
	std::unique_ptr<Sprite> finishSprite_;

	// スタート位置
	const Vector2 startPositon_ = { 2040.0f, 360.0f };
	// 中間位置
	const Vector2 middlePositon_ = { 640.0f, 360.0f };
	// エンド位置
	const Vector2 endPositon_ = { -760.0f, 360.0f };

	// スタート位置からか
	bool fromTheStart_;

	// 位置の媒介変数
	float postionT_;
	// クールタイム
	float coolTime_;
	// クールタイム中か
	bool isCooltime_;

	// スタート
	bool isStart_;
	// エンド
	bool isEnd_;

};

