#include "Target.h"
#include "../../Engine/base/TextureManager.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/Input/Input.h"
bool IsInnerCamera(const Vector3& vector) {
	if (std::abs(vector.x) <= 1.0f && std::abs(vector.y) <= 1.0f && vector.z > 0.0f) {
		return true;
	}
	return false;
}
void Target::Initialize(uint32_t textureHandle, uint32_t textureHandle2[2], uint32_t arrowTextureHandle, uint32_t lockonTextureHandle, uint32_t stickTextureHandle[2]) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Target";
	globalVariables->AddItem(groupName, "Range", targetRange_);
	globalVariables->AddItem(groupName, "ModePosition",modePosition_);
	globalVariables->AddItem(groupName, "ModeSize", modeSize_);
	anchor_.reset(Sprite::Create(textureHandle, { 0,0 }, {1.0f,1.0f,1.0f,1.0f}));
	ui_.reset(Sprite::Create(textureHandle2[0], {0,0}, {1.0f,1.0f,1.0f,1.0f}));
	textureHandle_[0] = textureHandle2[0];
	textureHandle_[1] = textureHandle2[1];
	isLockedChane_ = false;
	leftArrow_.reset(Sprite::Create(arrowTextureHandle, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }));
	Vector2 size = leftArrow_->GetSize();
	size.x /= 2.0f;
	size.y /= 2.0f;
	leftArrow_->SetSize(size);
	leftArrow_->SetIsFlipX(true);
	rightArrow_.reset(Sprite::Create(arrowTextureHandle, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }));
	rightArrow_->SetSize(size);
	modeText_.reset(Sprite::Create(lockonTextureHandle, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }));
	leftStick_.reset(Sprite::Create(stickTextureHandle[0], { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }));
	size = leftStick_->GetSize();
	size.x /= 2.0f;
	size.x /= 8.0f;
	size.y /= 8.0f;
	leftStick_->SetSize(size);
	leftStick_->SetTextureSize({ 320,256 });
	rightStick_.reset(Sprite::Create(stickTextureHandle[1], { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }));
	rightStick_->SetSize(size);
	rightStick_->SetTextureSize({ 320,256 });
	isDraw_ = true;
	isCanLockOn_ = true;
	isChangeTargetBlock_ = false;
}
void Target::Update(std::vector<Block*>* blockList, BaseCamera& camera, Player* player, std::list<std::unique_ptr<Screw>>* screwList) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Target";
	targetRange_ = globalVariables->GetFloatValue(groupName, "Range");
	modePosition_ = globalVariables->GetVector2Value(groupName, "ModePosition");
	modeSize_ = globalVariables->GetVector2Value(groupName, "ModeSize");
	modeText_->SetPosition(modePosition_);
	modeText_->SetSize(modeSize_);
	if (!isTarget_) {
		ForchNearAnchor(blockList, camera, player, screwList);
	}
	else {
		if (!AliveCheck(blockList) || targetBlock_->GetAnchorPointScrew(numTargetAnchor_)) {
			//isLockedChane_ = false;
			isTarget_ = false;
		}
		if (!isLockedChane_) {
			ForchNearAnchor(blockList, camera, player, screwList);
		}
		else {
			if (Input::GetInstance()->GetLeftAnalogstick().x != 0) {
				if (isReleseButton_) {
					int lr;
					if (Input::GetInstance()->GetLeftAnalogstick().x > 0.5f) {
						lr = 1;
						rightAnimation_ = 10.0f;
						ForchNearOneMore(blockList, camera, player, screwList, lr);
						isReleseButton_ = false;
						isChangeTargetBlock_ = true;
					}
					else if(Input::GetInstance()->GetLeftAnalogstick().x < -0.5f){
						lr = -1;
						leftAnimation_ = -10.0f;
						ForchNearOneMore(blockList, camera, player, screwList, lr);
						isReleseButton_ = false;
						isChangeTargetBlock_ = true;
					}
					//ForchNearOneMore(blockList, camera, player, screwList, lr);
					//isReleseButton_ = false;
				}
			}
			else {
				isReleseButton_ = true;
			}
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(targetBlock_->GetAnchorPointArray()[numTargetAnchor_].position, targetBlock_->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1));
			ui_->SetTextureHandle(textureHandle_[uiNum_]);
			anchor_->SetPosition({ pos.x,pos.y });
			anchor_->SetSize({ 256.0f / 4.0f,256.0f / 4.0f });
			ui_->SetPosition({ pos.x,pos.y - 64.0f });
			ui_->SetSize({ 135.0f,48.0f });
			if (!IsInnerCamera(newvp)) {
				//isLockedChane_ = false;
				isTarget_ = false;
			}
		}
	}
	if (isTarget_) {
		if (Input::GetInstance()->TriggerJoystick(JoystickButton::kJoystickButtonLB) && isCanLockOn_) {
			isLockedChane_ = !isLockedChane_;
			modeAlpha_ = 1.0f;
			alphaDirection_ = -1.0f;
		}
		
	}
	Vector2 position = anchor_->GetPosition();
	position.x += -100.0f + leftAnimation_;
	leftArrow_->SetPosition(position);
	position = anchor_->GetPosition();
	position.x += 100.0f + rightAnimation_;
	rightArrow_->SetPosition(position);

	leftAnimation_+=0.5f;
	if (leftAnimation_>0) {
		leftAnimation_ = 0;
	}
	rightAnimation_ -= 0.5f;
	if (rightAnimation_ < 0) {
		rightAnimation_ = 0;
	}

	modeAlpha_ += alphaDirection_ / 60.0f;
	if (modeAlpha_<0) {
		alphaDirection_ = 1.0f;
		modeAlpha_ = 0;
	}
	if (modeAlpha_ > 1.0f) {
		alphaDirection_ = -1.0f;
		modeAlpha_ = 1.0f;
	}
	modeText_->SetColor({1.0f,1.0f,1.0f,modeAlpha_});

	position = anchor_->GetPosition();
	position.x += -85.0f + leftAnimation_;
	position.y += 5.0f;
	leftStick_->SetPosition(position);
	leftStick_->SetTextureLeftTop({320*float(stickAnimation_),0});
	position = anchor_->GetPosition();
	position.x += 85.0f + rightAnimation_;
	position.y += 5.0f;
	rightStick_->SetPosition(position);
	rightStick_->SetTextureLeftTop({ 320 * float(stickAnimation_),0 });
	stickAnimationFrame_++;
	if (stickAnimationFrame_ >= 20) {
		stickAnimationFrame_ = 0;
		stickAnimation_ = int(bool(!stickAnimation_));
	}

	if (!isTarget_) {
		targetBlock_ = nullptr;
	}
}

bool Target::AliveCheck(std::vector<Block*>* blockList) {
	if (blockList->empty()) {
		return false;
	}
	std::vector<Block*>::iterator ite = blockList->begin();
	for (ite; ite != blockList->end(); ite++) {
		if (*ite == targetBlock_) {
			return true;
		}
	}
	return false;
}

void Target::ForchNearAnchor(std::vector<Block*>* blockList, BaseCamera& camera, Player* player, std::list<std::unique_ptr<Screw>>* screwList) {
	isTarget_ = false;
	if (blockList->empty()) {
		isTarget_ = false;
		return;
	}
	bool isFollow = false;
	for (std::list<std::unique_ptr<Screw>>::iterator ite = screwList->begin(); ite != screwList->end();ite++) {
		if ((*ite)->GetState() == Screw::FOLLOW) {
			isFollow = true;
		}
	}
	if (!isFollow) {
		//return;
	}

	std::vector<Block*>::iterator ite = blockList->begin();
	Block* block = *ite;
	size_t num = 0;
	//対象ブロックが一つだったとき
	/*if (blockList->size() == 1) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size();index++) {
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[index].position, block->GetWorldTransform()->worldMatrix_),camera.GetViewProjectionMatrix());
			bool lengthCheck = Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[num].position, camera.GetTransform()))
			>= Vector3Calc::Length(Vector3Calc::Subtract(block->GetAnchorPointArray()[index].position,camera.GetTransform()));
			if ((!IsInnerCamera(oldvp) || lengthCheck) && (IsInnerCamera(newvp))) {
				num = index;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1));
				anchor_->SetPosition({ pos.x,pos.y });
				anchor_->SetSize({0.5f,0.5f});
				ui_->SetPosition({ pos.x,pos.y-32.0f });
				ui_->SetSize({ 0.5f,0.5f });
				isTarget_ = true;
			}
		}
		targetBlock_ = block;
		numTargetAnchor_ = num;
		return;
		//現在の情報をいれてリターン
	}*/
	float oldDot=0;
	for (ite; ite != blockList->end();ite++) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size(); index++) {
			if ((*ite)->GetIsCenter()) {
				continue;
			}
			Matrix4x4 view = Matrix4x4Calc::Inverse( player->GetWorldTransform()->worldMatrix_);
			float fovY_ = 0.5f;
			float aspectRatio_ = 1.0f;
			float nearClip_ = 0.01f;
			float farClip_ = 100.0f;
			Matrix4x4 proj = Matrix4x4Calc::MakePerspectiveFovMatrix(fovY_,aspectRatio_,nearClip_,farClip_);
			Matrix4x4 vp = Matrix4x4Calc::Multiply(view,proj);
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			//Vector3 oldpl = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), vp);
			//Vector3 newpl = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), vp);
			bool lengthCheck/* = std::abs(newpl.x) <= std::abs(oldpl.x) && newpl.z > 0.0f*/;
			lengthCheck = true;
			//>= Vector3Calc::Length(Vector3Calc::Subtract((*ite)->GetAnchorPointArray()[index].position, camera.GetTransform()));
			float dot = Vector3Calc::Dot(Vector3Calc::Normalize(Vector3Calc::Subtract(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_),player->GetWorldTransform()->GetWorldPosition())),Vector3Calc::Normalize(player->GetDirection()));
			float oldlength = Vector3Calc::Length(Vector3Calc::Subtract(player->GetWorldTransform()->GetWorldPosition(), Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_)));
			float newlength = Vector3Calc::Length(Vector3Calc::Subtract(player->GetWorldTransform()->GetWorldPosition(), Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_)));
			if ((lengthCheck) && dot>0.8f  && IsInnerCamera(newvp) && (!isTarget_ || /*std::cos(dot) < std::cos(oldDot)*/ newlength <= oldlength) && !(*ite)->GetAnchorPointScrew(index) && newlength <= targetRange_ && (*ite)->GetIsRidePlayer() == false) {
				num = index;
				block = *ite;
				oldDot = dot;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)); 
				uiNum_ = 0;
				if (block->GetAnchorPointScrew(num)) {
					uiNum_ = 1;
				}
				ui_->SetTextureHandle(textureHandle_[uiNum_]);
				anchor_->SetPosition({ pos.x,pos.y });
				anchor_->SetSize({ 256.0f/4.0f,256.0f/4.0f });
				ui_->SetPosition({ pos.x,pos.y - 64.0f });
				ui_->SetSize({ 135.0f,48.0f });
				isTarget_ = true;
			}
		}
	}
	targetBlock_ = block;
	numTargetAnchor_ = num;
	return;
}

void Target::ForchNearOneMore(std::vector<Block*>* blockList, BaseCamera& camera, Player* player, std::list<std::unique_ptr<Screw>>* screwList, int lr) {
	//isTarget_ = false;
	if (blockList->empty()) {
		isTarget_ = false;
		return;
	}
	bool isFollow = false;
	for (std::list<std::unique_ptr<Screw>>::iterator ite = screwList->begin(); ite != screwList->end(); ite++) {
		if ((*ite)->GetState() == Screw::FOLLOW) {
			isFollow = true;
		}
	}
	if (!isFollow) {
		//return;
	}

	std::vector<Block*>::iterator ite = blockList->begin();
	Block* block = targetBlock_;
	Vector3 fstvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[numTargetAnchor_].position, block->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
	size_t num = numTargetAnchor_;
	
	float oldDot = 0;
	for (ite; ite != blockList->end(); ite++) {
		for (size_t index = 0; index < block->GetAnchorPointArray().size(); index++) {
			if ((*ite)->GetIsCenter()) {
				continue;
			}
			Vector3 oldvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			Vector3 newvp = Matrix4x4Calc::Transform(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), camera.GetViewProjectionMatrix());
			bool lengthCheck/* = std::abs(newpl.x) <= std::abs(oldpl.x) && newpl.z > 0.0f*/;
			//lengthCheck = true;
			float dot = Vector3Calc::Dot(Vector3Calc::Normalize(Vector3Calc::Subtract(Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_), player->GetWorldTransform()->GetWorldPosition())), Vector3Calc::Normalize(player->GetDirection()));
			float oldlength = Vector3Calc::Length(Vector3Calc::Subtract(player->GetWorldTransform()->GetWorldPosition(), Matrix4x4Calc::Transform(block->GetAnchorPointArray()[num].position, block->GetWorldTransform()->worldMatrix_)));
			float newlength = Vector3Calc::Length(Vector3Calc::Subtract(player->GetWorldTransform()->GetWorldPosition(), Matrix4x4Calc::Transform((*ite)->GetAnchorPointArray()[index].position, (*ite)->GetWorldTransform()->worldMatrix_)));
			float min = (std::min)(float(lr), 0.0f);
			float max = (std::max)(float(lr), 0.0f);
			float n = (fstvp.x - newvp.x) * float(lr);
			lengthCheck = (std::abs(fstvp.x - newvp.x) <  std::abs(fstvp.x -oldvp.x) || block == targetBlock_) && n<0;
			if ((lengthCheck) && IsInnerCamera(newvp) && newvp.z > 0.0f
				&& !(*ite)->GetAnchorPointScrew(index) && newlength <= targetRange_
				&& (*ite)->GetIsRidePlayer() == false && (*ite) != targetBlock_) {
				num = index;
				block = *ite;
				oldDot = dot;
				Vector3 pos = Matrix4x4Calc::Transform(newvp, Matrix4x4Calc::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1));
				uiNum_ = 0;
				if (block->GetAnchorPointScrew(num)) {
					uiNum_ = 1;
				}
				ui_->SetTextureHandle(textureHandle_[uiNum_]);
				anchor_->SetPosition({ pos.x,pos.y });
				anchor_->SetSize({ 256.0f / 4.0f,256.0f / 4.0f });
				ui_->SetPosition({ pos.x,pos.y - 64.0f });
				ui_->SetSize({ 135.0f,48.0f });
				isTarget_ = true;
			}
		}
	}
	targetBlock_ = block;
	numTargetAnchor_ = num;
	return;
}

void Target::SpriteDraw() {
	if (isTarget_ && isDraw_) {
		anchor_->Draw();
		ui_->Draw();
		if (isLockedChane_) {
			leftArrow_->Draw();
			rightArrow_->Draw();
			leftStick_->Draw();
			rightStick_->Draw();
		}
	}
	if (isLockedChane_) {
		modeText_->Draw();
	}
}