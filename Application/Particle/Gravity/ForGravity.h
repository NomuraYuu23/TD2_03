#pragma once
class ForGravity
{
public:
	static ForGravity* GetInstance() {
		static ForGravity instance;
		return &instance;
	}
	float radius_=0;
};
