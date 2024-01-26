#include "ShadowManager.h"

void ShadowManager::Initialize(Model* model)
{

	// モデル
	model_ = model;

	// リスト
	ListClear();

	// ワールドトランスフォーム
	for (uint32_t i = 0; i < worldTransforms_.size(); ++i) {
		worldTransforms_[i].Initialize();
	}

	// 影を出す数
	shadowCount_ = 0u;

}

void ShadowManager::ListClear()
{

	castsShadowObjList_.clear();
	shadowAppearsObjList_.clear();

}

void ShadowManager::CastsShadowObjListRegister(const Vector3& position, const Vector3& size)
{

	ShadowObj shadowObj = { position , size };

	castsShadowObjList_.push_back(shadowObj);

}

void ShadowManager::ShadowAppearsObjListRegister(const Vector3& position, const Vector3& size)
{

	ShadowObj shadowObj = { position , size };

	shadowAppearsObjList_.push_back(shadowObj);

}

void ShadowManager::SeeShadow()
{

	shadowCount_ = 0u;

	// 影を発生させるオブジェクトリスト
	std::list<ShadowObj>::iterator itrCastsShadowObj = castsShadowObjList_.begin();
	for (; itrCastsShadowObj != castsShadowObjList_.end(); ++itrCastsShadowObj) {

		// 影が現れるオブジェクトリスト	
		std::list<ShadowObj>::iterator itrShadowAppearsObj = shadowAppearsObjList_.begin();
		for (; itrShadowAppearsObj != shadowAppearsObjList_.end(); ++itrShadowAppearsObj) {

			// 影が出るか確認
			if (OverlapY(*itrCastsShadowObj, *itrShadowAppearsObj)) {
				// ワールドトランスフォーム設定
				worldTransforms_[shadowCount_].transform_.translate.x = itrCastsShadowObj->position_.x;
				worldTransforms_[shadowCount_].transform_.translate.y = itrShadowAppearsObj->position_.y + itrShadowAppearsObj->size_.y;
				worldTransforms_[shadowCount_].transform_.translate.z = itrCastsShadowObj->position_.z;
				worldTransforms_[shadowCount_].transform_.scale.x = itrCastsShadowObj->size_.x;
				worldTransforms_[shadowCount_].transform_.scale.y = 1.0f;
				worldTransforms_[shadowCount_].transform_.scale.z = itrCastsShadowObj->size_.z;
				worldTransforms_[shadowCount_].UpdateMatrix();

				// 影の数を増やす
				shadowCount_++;

				// 影の作成最大値
				if (shadowCount_ == kShadowMax_) {
					return;
				}

				break;
			}

		}

	}


}

void ShadowManager::Draw(BaseCamera& baseCamera)
{

	for (uint32_t i = 0; i < shadowCount_; ++i) {
		model_->Draw(worldTransforms_[i], baseCamera);
	}

}

bool ShadowManager::OverlapY(const ShadowObj& a, const ShadowObj& b)
{
	
	if (a.position_.x + a.size_.x >= b.position_.x - b.size_.x &&
		a.position_.x - a.size_.x <= b.position_.x + b.size_.x &&
		a.position_.z + a.size_.z >= b.position_.z - b.size_.z &&
		a.position_.z - a.size_.z <= b.position_.z + b.size_.z) {

		if (a.position_.y + a.size_.y >= b.position_.y + b.size_.y) {
			return true;
		}

	}

	return false;

}
