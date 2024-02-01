#pragma once
#include "../../../Engine/Math/Vector3.h"
class ForLinerEmitterData
{
public:
	static ForLinerEmitterData* GetInstance() {
		static ForLinerEmitterData instance;
		return &instance;
	};
	void SetData(const Vector3& s, const Vector3& e) { start_ = s; end_ = e; };
	Vector3 GetStart() { return start_; };
	Vector3 GetEnd() { return end_; };
	void SetIsDraw(bool is) { isDraw_ = is; };
	bool GetIsDraw() { return isDraw_; };
private:
	Vector3 start_ = {0};
	Vector3 end_ = {0};
	bool isDraw_=false;
};

