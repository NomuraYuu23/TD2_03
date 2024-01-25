#pragma once
#include <stdint.h>
class ForResult
{
public:
	static ForResult* GetInstance() {
		static ForResult instance;
		return &instance;
	}

	int32_t connectNum_;
};
