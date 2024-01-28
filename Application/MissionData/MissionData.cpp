#include "MissionData.h"
#include "../../Engine/Math/RandomEngine.h"
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

	missionMax_ = missionBlock_.size() + missionPoint_.size();
	clearMissionPointNum_ = 0;
}

void MissionData::Update(int32_t connectCount, const Vector3& playerWorldPosition) {
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
