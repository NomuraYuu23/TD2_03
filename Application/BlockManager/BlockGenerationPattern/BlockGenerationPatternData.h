#pragma once
#include "../../../Engine/Math/Vector3.h"

struct BlockGenerationPatternData
{

	Vector3 position_;
	Vector3 velocity_;

};

inline void to_json(nlohmann::json& json, const BlockGenerationPatternData& value) {
	json = nlohmann::json{
		{"position", value.position_ },
		{"velocity", value.velocity_ },
	};
}

inline void from_json(const nlohmann::json& json, BlockGenerationPatternData& value) {
	
	// キーを確認
	if (json.contains("position") && json["position"].is_array() &&
		json.contains("velocity") && json["velocity"].is_array()) {

		value.position_ = json["position"];
		value.velocity_ = json["velocity"];
	}

}
