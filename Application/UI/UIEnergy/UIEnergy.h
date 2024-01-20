#pragma once
#include "../UI.h"
class UIEnergy : public UI
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="groupName">グループ名</param>
	/// <param name="size">サイズ</param>
	/// <param name="leftTop">左上座標</param>
	/// <param name="energyMax">エナジー最大量</param>
	void Initialize(uint32_t textureHandle, const std::string& groupName, const Vector2& size, const Vector2& leftTop, float energyMax);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="energy">エナジー量</param>
	void Update(float energy);

private:

	// エナジー最大量
	float energyMax_;

	// サイズ
	Vector2 UISizeMax_;

};

