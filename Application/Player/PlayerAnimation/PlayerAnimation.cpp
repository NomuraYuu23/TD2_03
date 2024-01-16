#include "PlayerAnimation.h"

void PlayerAnimation::Initialize(WorldTransform* worldTransform)
{

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].Initialize();
	}

	playerWorldTransform_ = worldTransform;
	assert(playerWorldTransform_);

	worldTransforms_[kPlayerPartIndexBody].parent_ = playerWorldTransform_;
	worldTransforms_[kPlayerPartIndexLeftLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexLeftLeg].transform_.translate.x = -0.7f;
	worldTransforms_[kPlayerPartIndexRightLeg].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexRightLeg].transform_.translate.x = 0.7f;
	worldTransforms_[kPlayerPartIndexMagnet].parent_ = &worldTransforms_[kPlayerPartIndexBody];
	worldTransforms_[kPlayerPartIndexMagnet].transform_.translate.y = 1.2f;

}

void PlayerAnimation::Update()
{

	for (uint32_t i = 0; i < worldTransforms_.size(); i++) {
		worldTransforms_[i].UpdateMatrix();
	}

}

void PlayerAnimation::Draw(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models, BaseCamera& camera, Material* mat)
{

	for (uint32_t i = 0; i < models.size(); i++) {
		models[i]->Draw(worldTransforms_[i], camera, mat);
	}

}
