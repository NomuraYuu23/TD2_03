#pragma once
#include "../../Engine/Math/Vector3.h"
class MissionData
{
public:
	enum MissionType
	{
		BLOCKNUM,
		TOPOINT
	};

	struct MissionToPoint
	{
		Vector3 point;
		bool isClear_ = false;
	};

	static MissionData* GetInstance(){
		static MissionData instance;
		return &instance;
	}
	void Initialize();
	void Update(int32_t connectCount,const Vector3& playerWorldPosition);
	void SetMissionNumBlock(size_t num) {
		missionNumBlock_ = num;
	}
	size_t GetMissionNumBlock() {
		return missionNumBlock_;
	}
	void SetMissionNumPoint(size_t num) {
		missionNumPoint_ = num;
	}
	size_t GetMissionNumPoint() {
		return missionNumPoint_;
	}
	void SetMax(size_t max) { missionMax_ = max; };
	size_t GetMax() { return missionMax_; };
	bool IsMissionBlockBeenUpdate() { return isBlockBeenUpdate_; };
	bool IsMissionPointBeenUpdate() { return isPointBeenUpdate_; };
	size_t GetClearCount() { return missionNumBlock_ + clearMissionPointNum_; };
	std::vector<MissionToPoint>& GetMissionPointVector() { return missionPoint_; };

	std::vector<int32_t>& GetMissionBlockVector() { return missionBlock_; }

	bool IsCompleteBlock() { return isCompleteBlock_; }
	bool IsCompletePoint() { return isCompletePoint_; }

private:
	//size_t missionNum_;
	size_t missionMax_;

	std::vector<int32_t> missionBlock_;
	std::vector<MissionToPoint> missionPoint_;
	size_t missionNumBlock_;
	size_t missionNumPoint_;
	bool isBlockBeenUpdate_;//そのフレームでミッションが達成されたか
	bool isPointBeenUpdate_;//そのフレームでミッションが達成されたか
	bool isCompleteBlock_;//ブロックのミッションがすべてクリアされたか
	bool isCompletePoint_;//指定位置のミッションがすべてクリアされたか

	size_t clearMissionPointNum_;//クリアした位置ミッションの数
};
