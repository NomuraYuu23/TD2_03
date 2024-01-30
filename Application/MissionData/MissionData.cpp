#include "MissionData.h"
#include "../../Engine/Math/RandomEngine.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
void MissionData::Initialize() {
	missionNumBlock_=0;
	missionNumPoint_=0;
	missionBlock_.clear();
	missionPoint_.clear();

	missionBlock_.push_back(2);
	missionBlock_.push_back(6);
	missionBlock_.push_back(10);
	missionBlock_.push_back(14);
	missionBlock_.push_back(20);
	missionBlock_.push_back(24);
	missionBlock_.push_back(28);
	missionBlock_.push_back(32);
	missionBlock_.push_back(36);
	missionBlock_.push_back(40);

	missionPoint_.push_back({20.0f,1.0f,0.0f});
	missionPoint_.push_back({ 20.0f,1.0f,-20.0f });
	missionPoint_.push_back({ -80.0f,1.0f,0.0f });
	missionPoint_.push_back({ 20.0f,1.0f,80.0f });

	missionMax_ = missionBlock_.size() + missionPoint_.size();
	clearMissionPointNum_ = 0;
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Planet";

	globalVariables->AddItem(groupName, "0Position", missionPoint_[0].point);
	globalVariables->AddItem(groupName, "1Position", missionPoint_[1].point);
	globalVariables->AddItem(groupName, "2Position", missionPoint_[2].point);
	globalVariables->AddItem(groupName, "3Position", missionPoint_[3].point);
}

void MissionData::Update(int32_t connectCount, const Vector3& playerWorldPosition) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "Planet";

	missionPoint_[0].point = globalVariables->GetVector3Value(groupName, "0Position");
	missionPoint_[1].point = globalVariables->GetVector3Value(groupName, "1Position");
	missionPoint_[2].point = globalVariables->GetVector3Value(groupName, "2Position");
	missionPoint_[3].point = globalVariables->GetVector3Value(groupName, "3Position");

	isBlockBeenUpdate_ = false;
	isPointBeenUpdate_ = false;
	//ブロック数側の確認
	if (missionBlock_[missionNumBlock_] <= connectCount) {
		if (missionBlock_.size() - 1 > missionNumBlock_) {
			missionNumBlock_++;
			isBlockBeenUpdate_ = true;
		}
		else {
			isCompleteBlock_ = true;
		}
	}

	//指定座標
	if (clearMissionPointNum_ < missionPoint_.size()) {
		float length = Vector3Calc::Length(Vector3Calc::Subtract(missionPoint_[missionNumPoint_].point,playerWorldPosition));
		if (length <= 2.0f) {
			missionPoint_[missionNumPoint_].isClear_ = true;
			std::vector<size_t> point;
			for (size_t num = 0; num < missionPoint_.size();num++) {
				if (!missionPoint_[num].isClear_) {
					point.push_back(num);
				}
			}
			if (!point.empty()) {
				size_t nextTarget = point[size_t(RandomEngine::GetRandom(0.0f, float(point.size() - 1)))];
				point.clear();
				missionNumPoint_ = nextTarget;
			}
			
		}
	}

}
