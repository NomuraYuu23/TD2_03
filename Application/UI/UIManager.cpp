#include "UIManager.h"

void UIManager::Initialize(const std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	UIInitialize();

}

void UIManager::Update()
{

	for (uint32_t i = 0; i < i < UIIndex::kUIIndexOfCount; ++i) {
		UIs_[i]->Update();
	}

}

void UIManager::Draw()
{

	for (uint32_t i = 0; i < i < UIIndex::kUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}

}

void UIManager::UIInitialize()
{
}
