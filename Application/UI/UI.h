#pragma once
#include <memory>
#include <cstdint>
#include "../../Engine/2D/Sprite.h"

class UI
{

public: // メンバ関数

	virtual void Initialize(uint32_t textureHandle, const std::string& groupName);

	virtual void Initialize(uint32_t textureHandle, const std::string& groupName, const Vector2& size, const Vector2& leftTop);

	virtual void Update();

	virtual void Update(const Vector2& leftTop);

	void Draw();

private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();


private: // メンバ変数

	std::unique_ptr<Sprite> sprite_;

	Vector2 position_;

	float rotate_;

	Vector2 size_;

	std::string groupName_;

};

