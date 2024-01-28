#include "Pause.h"

void Pause::Initialize(const std::array<uint32_t, PauseTextureIndex::kPausingTextureIndexOfCount>& textureHandles)
{

	input_ = Input::GetInstance();
	isPause_ = false;
	pauseMenuSelect_ = PauseMenu::kPauseMenuReturnToGame;
	goToTheTitle_ = false;
	textureHandles_ = textureHandles;

	// スプライト
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector2 position = { 640.0f, 360.0f };
	Vector2 size = { 0.0f,0.0f };
	
	// フレーム
	frameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexFrame], position, color));
	
	// タイトルへ
	position = { 640.0f, 440.0f };
	goToTitleSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexGoToTitle], position, color));
	size = goToTitleSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	goToTitleSprite_->SetSize(size);
	goToTitlePositionY_ = position.y;

	// ゲームへ
	position = { 640.0f, 280.0f };
	returnToGameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexReturnToGame], position, color));
	size = returnToGameSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	returnToGameSprite_->SetSize(size);
	returnToGamePositionY_ = position.y;

	// リスタート
	position = { 640.0f, 360.0f };
	restartSprite_.reset(Sprite::Create(textureHandles_[PauseTextureIndex::kPausingTextureIndexRestart], position, color));
	size = restartSprite_->GetSize();
	size.x *= 0.8f;
	size.y *= 0.8f;
	restartSprite_->SetSize(size);
	restartPositionY_ = position.y;

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

	InputStick();

	PauseMenuOperation();

}

void Pause::Draw()
{

	if (isPause_) {
		backGroundSprite_->Draw();
		frameSprite_->Draw();
		choiceSprite_->Draw();
		goToTitleSprite_->Draw();
		returnToGameSprite_->Draw();
		restartSprite_->Draw();
		arrowSprite_->Draw();
	}

}

void Pause::PoseSwitching()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonSTART)) {
		if (isPause_) {
			isPause_ = false;
		}
		else {
			isPause_ = true;
			goToTheTitle_ = false;
			pauseMenuSelect_ = PauseMenu::kPauseMenuReturnToGame;
		}
		audioManager_->PlayWave(kGameAudioNameIndexPauseChice);
	}

}

void Pause::PauseMenuOperation()
{

	// メニュー移動(上)
	if (stickY_ > 0.0f) {
		pauseMenuSelect_--;
		if (pauseMenuSelect_ < 0) {
			pauseMenuSelect_ = PauseMenu::kCountOfPauseMenu - 1;
		}
		audioManager_->PlayWave(kGameAudioNameIndexPauseChice);
	}
	// メニュー移動(下)
	else if (stickY_ < 0.0f) {
		pauseMenuSelect_++;
		if (pauseMenuSelect_ == PauseMenu::kCountOfPauseMenu) {
			pauseMenuSelect_ = 0;
		}
		audioManager_->PlayWave(kGameAudioNameIndexPauseChice);
	}

	switch (pauseMenuSelect_)
	{
	case PauseMenu::kPauseMenuReturnToGame:
		PauseMenuReturnToGame();
		break;
	case PauseMenu::kPauseMenuRestart:
		PauseMenuRestart();
		break;
	case PauseMenu::kPauseMenuGoToTitle:
		PauseMenuGoToTitle();
		break;
	default:
		break;
	}

}

void Pause::PauseMenuGoToTitle()
{

	Vector2 position = choiceSprite_->GetPosition();
	position.y = goToTitlePositionY_;
	choiceSprite_->SetPosition(position);
	
	position = arrowSprite_->GetPosition();
	position.y = goToTitlePositionY_;
	arrowSprite_->SetPosition(position);

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		goToTheTitle_ = true;
		audioManager_->PlayWave(kGameAudioNameIndexPauseDesicion);
	}

}

void Pause::PauseMenuReturnToGame()
{

	Vector2 position = choiceSprite_->GetPosition();
	position.y = returnToGamePositionY_;
	choiceSprite_->SetPosition(position);

	position = arrowSprite_->GetPosition();
	position.y = returnToGamePositionY_;
	arrowSprite_->SetPosition(position);

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		isPause_ = false;
		audioManager_->PlayWave(kGameAudioNameIndexPauseChice);
	}

}

void Pause::PauseMenuRestart()
{

	Vector2 position = choiceSprite_->GetPosition();
	position.y = restartPositionY_;
	choiceSprite_->SetPosition(position);

	position = arrowSprite_->GetPosition();
	position.y = restartPositionY_;
	arrowSprite_->SetPosition(position);

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		audioManager_->PlayWave(kGameAudioNameIndexPauseDesicion);
	}

}

void Pause::InputStick()
{

	if (stickColltime_ != 0.0f) {
		stickColltime_ -= 1.0f / 15.0f;
		if (stickColltime_ <= 0.0f) {
			stickColltime_ = 0.0f;
		}
		else {
			stickY_ = 0.0f;
			return;
		}
	}

	stickY_ = -input_->GetLeftAnalogstick().y;

	if (stickY_ != 0.0f) {
		//　クールタイム開始
		stickColltime_ = 1.0f;
	}

}
