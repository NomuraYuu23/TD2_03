#include "TutorialUIManager.h"

void TutorialUIManager::Initialize(const std::array<uint32_t, TutorialUITextureHandleIndex::kTutorialUITextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIInitPositions_[i] = UIs_[i]->GetPosition();
	}

}

void TutorialUIManager::Update()
{

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIs_[i]->Update();
	}

}

void TutorialUIManager::Draw()
{

	for (uint32_t i = 0; i < TutorialUIIndex::kTutorialUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}

}

void TutorialUIManager::UIInitialize()
{

	Vector2 size = { 1000.0f, 1000.0f };
	Vector2 leftTop = { 0.0f, 0.0f };
	std::string jsonName = "TutorialUI";

	size = {1280.0f, 720.0f};
	UIs_[kTutorialUIIndexFrame] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexFrame]->Initialize(textureHandles_[kTutorialUITextureHandleIndexFrame], "Frame", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionFrameLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionFrameLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionFrame], "MissionFrameLeft", size, leftTop, jsonName);

	size = { 800.0f, 700.0f };
	UIs_[kTutorialUIIndexMissionClearLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionClearLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionClear], "MissionClearLeft", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionTextLeft] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionTextLeft]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionTextMove], "MissionTextLeft", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionFrameRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionFrameRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionFrame], "MissionFrameRight", size, leftTop, jsonName);

	size = { 800.0f, 700.0f };
	UIs_[kTutorialUIIndexMissionClearRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionClearRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionClear], "MissionClearRight", size, leftTop, jsonName);

	size = { 1748.0f, 300.0f };
	UIs_[kTutorialUIIndexMissionTextRight] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexMissionTextRight]->Initialize(textureHandles_[kTutorialUITextureHandleIndexMissionTextCamera], "MissionTextRight", size, leftTop, jsonName);

	size = { 900.0f, 125.0f };
	UIs_[kTutorialUIIndexSkipText] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipText]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipText], "SkipText", size, leftTop, jsonName);

	size = { 900.0f, 125.0f };
	UIs_[kTutorialUIIndexSkipGage] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipGage]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipGage], "SkipGage", size, leftTop, jsonName);

	size = { 900.0f, 125.0f };
	UIs_[kTutorialUIIndexSkipGageFrame] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexSkipGageFrame]->Initialize(textureHandles_[kTutorialUITextureHandleIndexSkipGageFrame], "SkipGageFrame", size, leftTop, jsonName);

	size = { 320.0f, 256.0f };
	UIs_[kTutorialUIIndexStickL] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexStickL]->Initialize(textureHandles_[kTutorialUITextureHandleIndexStickL], "StickL", size, leftTop, jsonName);

	size = { 320.0f, 256.0f };
	UIs_[kTutorialUIIndexStickR] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexStickR]->Initialize(textureHandles_[kTutorialUITextureHandleIndexStickR], "StickR", size, leftTop, jsonName);

	size = { 280.0f, 192.0f };
	UIs_[kTutorialUIIndexButtonLB] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonLB]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonLB], "ButtonLB", size, leftTop, jsonName);

	size = { 280.0f, 192.0f };
	UIs_[kTutorialUIIndexButtonRB] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonRB]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonRB], "ButtonRB", size, leftTop, jsonName);

	size = { 450.0f, 450.0f };
	UIs_[kTutorialUIIndexButtonA] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonA]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonA], "ButtonA", size, leftTop, jsonName);

	size = { 450.0f, 450.0f };
	UIs_[kTutorialUIIndexButtonY] = std::make_unique<UI>();
	UIs_[kTutorialUIIndexButtonY]->Initialize(textureHandles_[kTutorialUITextureHandleIndexButtonY], "ButtonY", size, leftTop, jsonName);

}
