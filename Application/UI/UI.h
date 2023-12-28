#pragma once
#include <memory>
#include <cstdint>
#include "../../Engine/2D/Sprite.h"

class UI
{

public: // メンバ関数

	virtual void Initialize(uint32_t textureHandle);

	virtual void Update() = 0;

	void Draw();

private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables() = 0;

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables() = 0;


private: // メンバ変数

	std::unique_ptr<Sprite> sprite_;

	Vector2 position_;

	float rotate_;

	Vector2 size_;

};

