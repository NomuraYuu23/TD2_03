#pragma once

#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/Camera/BaseCamera.h"
#include "../../Engine/Math/Vector3.h"
#include "../Screw.h"
#include <array>
class Block {
public:

	struct AnchorPoint
	{
		Vector3 position;//立方体上の位置
		//bool isStings;//刺さってるか
		Screw* screw;//ネジのポインタ
	};

	void Initialize();
	void Update();
	void Draw(Model* model,BaseCamera& camera);

	std::array<AnchorPoint, 4>& GetAnchorPointArray() { return anchorPoints_; };
	Vector3 GetAnchorPointWorldPosition(size_t num);
	Screw* GetAnchorPointScrew(size_t num) { return anchorPoints_[num].screw; };
	Screw* SetAnchorPointScrew(size_t num,Screw* screw) { return anchorPoints_[num].screw = screw; };
private:

	std::array<AnchorPoint,4> anchorPoints_;
	WorldTransform worldTransform_;

};