#pragma once
#include <stdint.h>
class ForResult
{
public:
	static ForResult* GetInstance() {
		static ForResult instance;
		return &instance;
	}

	int32_t connectNum_=0;
	int32_t connectMax_=0;//最大同時接続数
};
