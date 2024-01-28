#include "Pause.h"

void Pause::Initialize(const std::array<uint32_t, PauseTextureIndex::kPausingTextureIndexOfCount>& textureHandles)
{

	input_ = Input::GetInstance();
	isPause_ = false;
	pauseMenuSelect_ = PauseMenu::kReturnToGame;
	goToTheTitle_ = false;
	textureHandles_ = textureHandles;

	// スプライト
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector2 position = { 640.0f, 360.0f };
	Vector2 size = { 0.0f,0.0f };
	
	// フレーム
	frameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexFrame], position, color));
	
	// タイトルへ
	position = { 640.0f, 280.0f };
	goToTitleSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexGoToTitle], position, color));
	size = goToTitleSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	goToTitleSprite_->SetSize(size);
	goToTitlePositionY = position.y;

	// ゲームへ
	position = { 640.0f, 500.0f };
	returnToGameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexReturnToGame], position, color));
	size = returnToGameSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	returnToGameSprite_->SetSize(size);
	returnToGamePositionY = position.y;

	// 矢印
	position = { 360.0f, 500.0f };
	arrowSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexArrow], position, color));
	size = arrowSprite_->GetSize();
	size.x *= 0.5f;
	size.y *= 0.5f;
	arrowSprite_->SetSize(size);

	// 選択
	position = { 640.0f, 500.0f };
	choiceSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexChoice], position, color));
	size = choiceSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	choiceSprite_->SetSize(size);

	// 背景
	position = { 640.0f, 360.0f };
	color = { 0.0f,0.0f,0.0f,0.8f };
	backGroundSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexBackGround], position, color));
	size = {1280.0f, 720.0f};
	backGroundSprite_->SetSize(size);

}

void Pause::Update()
{

	PoseSwitching();

	if (isPause_) {
		PauseMenuOperation();
	}

}

void Pause::Draw()
{

	if (isPause_) {
		backGroundSprite_->Draw();
		frameSprite_->Draw();
		choiceSprite_->Draw();
		goToTitleSprite_->Draw();
		returnToGameSprite_->Draw();
		arrowSprite_->Draw();
	}

}

void Pause::PoseSwitching()
{

	if (input_->TriggerKey(DIK_TAB)) {
		if (isPause_) {
			isPause_ = false;
		}
		else {
			isPause_ = true;
			goToTheTitle_ = false;
			pauseMenuSelect_ = PauseMenu::kReturnToGame;
		}
	}

}

void Pause::PauseMenuOperation()
{

	// メニュー移動(上)
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		pauseMenuSelect_--;
		if (pauseMenuSelect_ < 0) {
			pauseMenuSelect_ = PauseMenu::kCountOfPauseMenu - 1;
		}
	}
	// メニュー移動(下)
	else if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		pauseMenuSelect_++;
		if (pauseMenuSelect_ == PauseMenu::kCountOfPauseMenu) {
			pauseMenuSelect_ = 0;
		}
	}

	switch (pauseMenuSelect_)
	{
	case PauseMenu::kGoToTitle:
		PauseMenuGoToTitle();
		break;
	case PauseMenu::kReturnToGame:
		PauseMenuReturnToGame();
		break;
	default:
		break;
	}

}

void Pause::PauseMenuGoToTitle()
{

	Vector2 position = choiceSprite_->GetPosition();
	position.y = goToTitlePositionY;
	choiceSprite_->SetPosition(position);
	
	position = arrowSprite_->GetPosition();
	position.y = goToTitlePositionY;
	arrowSprite_->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE)) {
		goToTheTitle_ = true;
	}

}

void Pause::PauseMenuReturnToGame()
{

	Vector2 position = choiceSprite_->GetPosition();
	position.y = returnToGamePositionY;
	choiceSprite_->SetPosition(position);

	position = arrowSprite_->GetPosition();
	position.y = returnToGamePositionY;
	arrowSprite_->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE)) {
		isPause_ = false;
	}

}
