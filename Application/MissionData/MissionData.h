#pragma once
class MissionData
{
public:
	static MissionData* GetInstance(){
		static MissionData instance;
		return &instance;
	}
	void Initialize() {
		missionNum_ = 0;
	};
	void SetMissionNum(size_t num) {
		missionNum_ = num;
	}
	size_t GetMissionNum() {
		return missionNum_;
	}
private:
	size_t missionNum_;
};
