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

	struct MissionBlock
	{
		int32_t num;
		bool isClear_ = false;
		size_t addScrewNum_;
	};

	struct MissionToPoint
	{
		Vector3 point;
		bool isClear_ = false;
		size_t addScrewNum_;
		float rotateY;
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
	size_t GetClearCount() { return clearMissionBlockNum_ + clearMissionPointNum_; };
	std::vector<MissionToPoint>& GetMissionPointVector() { return missionPoint_; };

	std::vector<MissionBlock>& GetMissionBlockVector() { return missionBlock_; }

	bool IsCompleteBlock() { return isCompleteBlock_; }
	bool IsCompletePoint() { return isCompletePoint_; }
	size_t GetAddScrewNumBlock() { return addScrewNumBlock_; };
	size_t GetAddScrewNumPoint() { return addScrewNumPoint_; };
private:
	//size_t missionNum_;
	size_t missionMax_;

	std::vector<MissionBlock> missionBlock_;
	std::vector<MissionToPoint> missionPoint_;
	size_t missionNumBlock_;
	size_t missionNumPoint_;
	bool isBlockBeenUpdate_;//そのフレームでミッションが達成されたか
	bool isPointBeenUpdate_;//そのフレームでミッションが達成されたか
	bool isCompleteBlock_;//ブロックのミッションがすべてクリアされたか
	bool isCompletePoint_;//指定位置のミッションがすべてクリアされたか

	size_t clearMissionBlockNum_;//クリアしたブロックミッションの数
	size_t clearMissionPointNum_;//クリアした位置ミッションの数
	size_t addScrewNumBlock_=0;
	size_t addScrewNumPoint_ = 0;
};
