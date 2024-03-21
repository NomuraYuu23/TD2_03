#include "MissionData.h"
#include "../../Engine/Math/RandomEngine.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
void MissionData::Initialize() {
	missionNumBlock_=0;
	missionNumPoint_=0;
	missionBlock_.clear();
	missionPoint_.clear();

	missionBlock_.push_back({  2 ,false,2 });
	missionBlock_.push_back({  4 ,false,2 });
	missionBlock_.push_back({  6 ,false,2 });
	missionBlock_.push_back({  8 ,false,2 });
	missionBlock_.push_back({ 10 ,false,2 });
	missionBlock_.push_back({ 12 ,false,2 });
	missionBlock_.push_back({ 14 ,false,2 });
	missionBlock_.push_back({ 16 ,false,2 });
	missionBlock_.push_back({ 20 ,false,2 });
	missionBlock_.push_back({ 26 ,false,2 });

	missionPoint_.push_back({ { 20.0f,1.0f, 0.0f },false,4 });
	missionPoint_.push_back({ { 20.0f,1.0f,20.0f },false,4 });
	missionPoint_.push_back({ {-80.0f,1.0f, 0.0f },false,2 });
	missionPoint_.push_back({ { 20.0f,1.0f,80.0f },false,2 });

	missionMax_ = missionBlock_.size() + missionPoint_.size();
	clearMissionBlockNum_ = 0;
	clearMissionPointNum_ = 0;
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Planet";

	globalVariables->AddItem(groupName, "0Position", missionPoint_[0].point);
	globalVariables->AddItem(groupName, "1Position", missionPoint_[1].point);
	globalVariables->AddItem(groupName, "2Position", missionPoint_[2].point);
	globalVariables->AddItem(groupName, "3Position", missionPoint_[3].point);
	globalVariables->AddItem(groupName, "0RotateY", 0.0f);
	globalVariables->AddItem(groupName, "1RotateY", 0.0f);
	globalVariables->AddItem(groupName, "2RotateY", 0.0f);
	globalVariables->AddItem(groupName, "3RotateY", 0.0f);
	isCompleteBlock_ = false;
	isCompletePoint_ = false;
	isBlockBeenUpdate_ = false;
	isPointBeenUpdate_ = false;

	flagSprite_.reset(
		Sprite::Create(flagTextureHandles_[0],
			{ 0,0 }, {1.0f,1.0f,1.0f,1.0f}));
	Vector2 size = { 512.0f , 512.0f };
	size.x *= 0.15f;
	size.y *= 0.15f;
	flagSprite_->SetSize(size);
}

void MissionData::Update(int32_t connectCount, const Vector3& playerWorldPosition) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Planet";

	missionPoint_[0].point = globalVariables->GetVector3Value(groupName, "0Position");
	missionPoint_[1].point = globalVariables->GetVector3Value(groupName, "1Position");
	missionPoint_[2].point = globalVariables->GetVector3Value(groupName, "2Position");
	missionPoint_[3].point = globalVariables->GetVector3Value(groupName, "3Position");
	missionPoint_[0].rotateY = globalVariables->GetFloatValue(groupName, "0RotateY");
	missionPoint_[1].rotateY = globalVariables->GetFloatValue(groupName, "1RotateY");
	missionPoint_[2].rotateY = globalVariables->GetFloatValue(groupName, "2RotateY");
	missionPoint_[3].rotateY = globalVariables->GetFloatValue(groupName, "3RotateY");

	addScrewNumBlock_ = 0;
	addScrewNumPoint_ = 0;
	isBlockBeenUpdate_ = false;
	isPointBeenUpdate_ = false;
	//ブロック数側の確認
	if (missionBlock_[missionNumBlock_].num <= connectCount) {
		missionBlock_[missionNumBlock_].isClear_ = true;
		addScrewNumBlock_ += missionBlock_[missionNumBlock_].addScrewNum_;
		if (missionBlock_.size() - 1 > missionNumBlock_) {
			missionNumBlock_++;
			clearMissionBlockNum_++;
			isBlockBeenUpdate_ = true;
		}
		else  if (!isCompleteBlock_) {
			isCompleteBlock_ = true;
			isBlockBeenUpdate_ = true;
			clearMissionBlockNum_++;
		}
	}

//	isDrawFlagUI_ = false;
	//指定座標
	if (clearMissionPointNum_ < missionPoint_.size()) {
		//isDrawFlagUI_ = true;
		float length = Vector3Calc::Length(Vector3Calc::Subtract(missionPoint_[missionNumPoint_].point,playerWorldPosition));
		if (length <= 2.0f) {
			missionPoint_[missionNumPoint_].isClear_ = true;
			addScrewNumPoint_ += missionPoint_[missionNumPoint_].addScrewNum_;
			std::vector<size_t> point;
			for (size_t num = 0; num < missionPoint_.size();num++) {
				if (!missionPoint_[num].isClear_) {
					point.push_back(num);
				}
			}
			if (!point.empty()) {
				//size_t nextTarget = point[size_t(RandomEngine::GetRandom(0.0f, float(point.size() - 1)))];
				point.clear();
				//missionNumPoint_ = nextTarget;
				missionNumPoint_++;
				isPointBeenUpdate_ = true;
				clearMissionPointNum_++;
			}
			else if(!isCompletePoint_){
				missionNumPoint_++;
				isCompletePoint_ = true;
				isPointBeenUpdate_ = true;
				clearMissionPointNum_++;
			}
			
		}
	}

}

void MissionData::FlagUIUpdate(BaseCamera& camera) {
	isDrawFlagUI_ = false;
	if (clearMissionPointNum_ < missionPoint_.size()) {
		Matrix4x4 missionWorld = Matrix4x4Calc::MakeTranslateMatrix(missionPoint_[missionNumPoint_].point);
		Vector3 pos = Matrix4x4Calc::Transform(missionPoint_[missionNumPoint_].point, camera.GetViewProjectionMatrix());
		
		flagSprite_->SetTextureHandle(flagTextureHandles_[missionNumPoint_]);
		if (pos.x < -1.0f || (pos.y < -1.0f) || (pos.y > 1.0f) || pos.x > 1.0f || pos.z<0) {
			isDrawFlagUI_ = true;
			pos = Matrix4x4Calc::Transform(pos, Matrix4x4Calc::MakeViewportMatrix(0,0,1280.0f,720.0f,0.0f,1.0f));
			pos.x = std::clamp(pos.x,64.0f,1280.0f-64.0f);
			pos.y = std::clamp(pos.y, 64.0f, 720.0f - 64.0f);
			flagSprite_->SetPosition({ pos.x,pos.y });
		}
	}
}

void MissionData::DrawFlagUI() {
	if (isDrawFlagUI_) {
		flagSprite_->Draw();
	}
}