#include "BlockManager.h"
#include "../Block/Block.h"
#include "../Player/player.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "BlockManagerState/BlockManagerStateInit/BlockManagerStateInit.h"
#include <random>

BlockManager::~BlockManager()
{

	for (Block* block : blocks_) {
		delete block;
	}

	blocks_.remove(nullptr);

}

void BlockManager::Initialize(Model* model)
{

	assert(model);

	// ブロック
	blocks_.clear();

	// ブロックの最大位置
	maxBlockPosition_ = {1000.0f, 1000.0f, 1000.0f };

	// ブロックの最小位置
	minBlockPosition_ = { -1000.0f, -1000.0f, -1000.0f };

	// ブロックの最大数
	maxBlock_ = 64;

	// ブロック生成のクールタイムカウント(フレーム)
	generationBlockFrameCount_ = 0u;

	// ブロック生成のクールタイム(フレーム)
	generationBlockFrame_ = 128;

	// ブロックのサイズ
	blockSize_[kBlockSizeIndexSmall] = { 1.0f, 1.0f, 1.0f };
	blockSize_[kBlockSizeIndexMiddle] = { 3.0f, 3.0f, 3.0f };
	blockSize_[kBlockSizeIndexBig] = { 5.0f, 5.0f, 5.0f };

	// モデル
	model_ = model;

	// マテリアル
	MaterialInitialize();

	// 外部変数登録
	RegisteringGlobalVariables();

	// 外部変数適用
	ApplyGlobalVariables();


	// ブロック
	
	GenerationCenterBlock();
	Vector3 position = { 2.0f, 0.0f, 60.0f };
	Vector3 velocity = { 0.1f, 0.0f, 0.0f };
	GenerationBlock(position, velocity);

	position = { -200.0f,0.0f,-30.0f };
	GenerationBlock(position, velocity);

	position = { -230.0f,0.0f,-50.0f };
	GenerationBlock(position, velocity);

	position = { -250.0f,0.0f,-30.0f };
	GenerationBlock(position, velocity);

	position = { -300.0f,0.0f,-50.0f };
	GenerationBlock(position, velocity);

	position = { -320.0f,0.0f,-30.0f };
	GenerationBlock(position, velocity);

	position = { -340.0f,0.0f,-60.0f };
	GenerationBlock(position, velocity);

	position = { -400.0f,0.0f,-80.0f };
	GenerationBlock(position, velocity);

	position = { -420.0f,0.0f,-30.0f };
	GenerationBlock(position, velocity);

	position = { -440.0f,0.0f,-70.0f };
	GenerationBlock(position, velocity);

}

void BlockManager::Update()
{

#ifdef _DEBUG

	// 外部変数適用
	ApplyGlobalVariables();

#endif // _DEBUG

	//ブロックの範囲制御
	RangeControl();
	
	if (generationBlockFrameCount_++ >= generationBlockFrame_) {
		generationBlockFrameCount_ = 0;
		
		// ブロックの数制御
		uint32_t newBlockNum = NumberControl();
		if (generatedBlocksNumMax_ > newBlockNum) {
			generatedBlocksNum_ = newBlockNum;
		}
		else {
			generatedBlocksNum_ = generatedBlocksNumMax_;
		}

		// プレイヤーの位置
		Vector3 playerPosition = player_->GetWorldTransform()->GetWorldPosition();

		// ブロック生成
		for (uint32_t i = 0; i < generatedBlocksNum_; i++) {

			// どこから来るか
			blockGenerationDirection_ = WhereComeFrom(playerPosition);

			// 座標は
			Vector3 position = CreatePosition(blockGenerationDirection_, playerPosition);

			// 速度は
			Vector3 velocity = CreateVelocity(blockGenerationDirection_);

			// 生成
			GenerationBlock(position, velocity);

		}
	}

}

void BlockManager::Draw(BaseCamera& camera)
{

	for (Block* block : blocks_) {
		// ブロックの描画
		block->Draw(model_, camera);
	}

}

void BlockManager::ImGuiDraw()
{
}

void BlockManager::RangeControl()
{

	Vector3 playerPosition = player_->GetWorldTransform()->GetWorldPosition();

	blocks_.remove_if([=](Block* block) {
		
		// ブロックの位置取得
		Vector3 blockPosition = block->GetAnchorPointWorldPosition(0);
		
		// ブロックが範囲を超えていたら削除
		if (blockPosition.x > playerPosition.x + maxBlockPosition_.x ||
			blockPosition.y > playerPosition.y + maxBlockPosition_.y ||
			blockPosition.z > playerPosition.z + maxBlockPosition_.z ||
			blockPosition.x < playerPosition.x + minBlockPosition_.x ||
			blockPosition.y < playerPosition.y + minBlockPosition_.y ||
			blockPosition.z < playerPosition.z + minBlockPosition_.z) {
			delete block;
			return true;
		}
		
		return false;
		
		});

}

uint32_t BlockManager::NumberControl()
{

	// 現在のブロックの数
	uint32_t blockNum = static_cast<uint32_t>(blocks_.size());

	// 生成できるブロックの数
	uint32_t maxGenerationBlock = maxBlock_ - blockNum;

	return maxGenerationBlock;

}

BlockGenerationDirection BlockManager::WhereComeFrom(const Vector3& position)
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 横か
	bool beside = false;

	// 横か縦か
	std::uniform_real_distribution<float> distributionDirection(-1.0f, 1.0f);
	beside = distributionDirection(randomEngine) >= 0.0f;

	// 確定ライン
	float confirmedLine = 50.0f;
	//x
	if (beside) {
		// 東か
		if (position.x >= confirmedLine) {
			return kBlockGenerationDirectionEast;
		}
		else if(position.x <= -confirmedLine){
			return kBlockGenerationDirectionWest;
		}
		else {
			std::uniform_real_distribution<float> distributionBeside(-1.0f, 1.0f);
			if (distributionBeside(randomEngine) >= 0.0f) {
				return kBlockGenerationDirectionEast;
			}
			else {
				return kBlockGenerationDirectionWest;
			}
		}
	}
	else {
		// 北か
		if (position.y >= confirmedLine) {
			return kBlockGenerationDirectionNorth;
		}
		else if (position.y <= -confirmedLine) {
			return kBlockGenerationDirectionSouth;
		}
		else {
			std::uniform_real_distribution<float> distributionVertical(-1.0f, 1.0f);
			if (distributionVertical(randomEngine) >= 0.0f) {
				return kBlockGenerationDirectionNorth;
			}
			else {
				return kBlockGenerationDirectionSouth;
			}
		}
	}

}

Vector3 BlockManager::CreatePosition(BlockGenerationDirection blockGenerationDirection, const Vector3& position)
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	Vector3 result = {};

	result.y = 0.0f;

	// 幅
	std::uniform_real_distribution<float> distributionPosition(-generatedWidth_, generatedWidth_);

	switch (blockGenerationDirection)
	{
	case kBlockGenerationDirectionNorth:
		result.x = position.x + distributionPosition(randomEngine);
		result.z = position.z + generatedWidth_;
		break;
	case kBlockGenerationDirectionSouth:
		result.x = position.x + distributionPosition(randomEngine);
		result.z = position.z - generatedWidth_;
		break;
	case kBlockGenerationDirectionEast:
		result.x = position.x + generatedWidth_;
		result.z = position.z + distributionPosition(randomEngine);
		break;
	case kBlockGenerationDirectionWest:
		result.x = position.x - generatedWidth_;
		result.z = position.z + distributionPosition(randomEngine);
		break;
	default:
		break;
	}

	return result;

}

Vector3 BlockManager::CreateVelocity(BlockGenerationDirection blockGenerationDirection)
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	Vector3 result = {};

	// 速度
	float speed = blockSpped_[0];
	std::uniform_real_distribution<float> distributionSpeed(0, 1.0f);
	float SpeedLottery = distributionSpeed(randomEngine);
	if (SpeedLottery < 0.33f) {
		speed = blockSpped_[0];
	}
	else if (SpeedLottery < 0.66f) {
		speed = blockSpped_[1];
	}
	else {
		speed = blockSpped_[2];
	}

	switch (blockGenerationDirection)
	{
	case kBlockGenerationDirectionNorth:
		result.z = -speed;
		break;
	case kBlockGenerationDirectionSouth:
		result.z = speed;
		break;
	case kBlockGenerationDirectionEast:
		result.x = -speed;
		break;
	case kBlockGenerationDirectionWest:
		result.x = speed;
		break;
	default:
		break;
	}

	return result;

}

void BlockManager::GenerationBlock(const Vector3& position, const Vector3& velocity)
{

	Block* newBlock = new Block();
	newBlock->Initialize();
	newBlock->SetWorldPosition(position);
	newBlock->SetVelocity(velocity);
	blocks_.push_back(newBlock);

}

void BlockManager::GenerationCenterBlock()
{

	Block* newBlock = new Block();
	newBlock->Initialize();
	newBlock->SetIsCenter(true);
	newBlock->SetIsConnect(true);
	newBlock->SetVelocity({ 0.0f,0.0f,-0.1f });
	newBlock->SetSize({4.0f,2.0f,4.0f});
	blocks_.push_back(newBlock);

}

void BlockManager::GenerationCenterBlock(const Vector3& position, const Vector3& size) {
	Block* newBlock = new Block();
	newBlock->Initialize();
	newBlock->SetIsCenter(true);
	newBlock->SetIsConnect(true);
	newBlock->SetWorldPosition(position);
	newBlock->SetSize(size);
	blocks_.push_back(newBlock);
}

void BlockManager::MaterialInitialize()
{


	// マテリアル生成
	for (uint32_t i = 0; i < kBlockMaterialIndexOfCount; ++i) {
		materials_[i].reset(Material::Create());
	}

	// UVトランスフォーム（とりあえずいじらない）
	TransformStructure transform = {
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f }
	};
	// 色（黒）
	Vector4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	// ライティングをハーフランバートに
	int enableLighting = HalfLambert;
	// 輝き（ハーフランバートなので今のところ意味なし）
	float shininess = 100.0f;

	// 白
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materials_[kBlockMaterialIndexWhite]->Update(transform, color, enableLighting, shininess);

}

void BlockManager::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "BlockManager";

	globalVariables->AddItem(groupName, "maxBlockPosition", maxBlockPosition_);
	globalVariables->AddItem(groupName, "minBlockPosition", minBlockPosition_);
	globalVariables->AddItem(groupName, "maxBlock", maxBlock_);

	globalVariables->AddItem(groupName, "blockSize Small", blockSize_[kBlockSizeIndexSmall]);
	globalVariables->AddItem(groupName, "blockSize Middle", blockSize_[kBlockSizeIndexMiddle]);
	globalVariables->AddItem(groupName, "blockSize Big", blockSize_[kBlockSizeIndexBig]);

	globalVariables->AddItem(groupName, "generationBlockFrame", generationBlockFrame_);

	globalVariables->AddItem(groupName, "generatedWidth_", generatedWidth_);

	globalVariables->AddItem(groupName, "blockSpeed Fast", blockSpped_[kBlockSpeedIndexFast]);
	globalVariables->AddItem(groupName, "blockSpeed Middle", blockSpped_[kBlockSpeedIndexMiddle]);
	globalVariables->AddItem(groupName, "blockSpeed Slow", blockSpped_[kBlockSpeedIndexSlow]);

}

void BlockManager::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string groupName = "BlockManager";

	maxBlockPosition_ = globalVariables->GetVector3Value(groupName, "maxBlockPosition");
	minBlockPosition_ = globalVariables->GetVector3Value(groupName, "minBlockPosition");
	maxBlock_ = globalVariables->GetUIntValue(groupName, "maxBlock");

	blockSize_[kBlockSizeIndexSmall] = globalVariables->GetVector3Value(groupName, "blockSize Small");
	blockSize_[kBlockSizeIndexMiddle] = globalVariables->GetVector3Value(groupName, "blockSize Middle");
	blockSize_[kBlockSizeIndexBig] = globalVariables->GetVector3Value(groupName, "blockSize Big");

	generationBlockFrame_ = globalVariables->GetUIntValue(groupName, "generationBlockFrame");

	generatedWidth_ = globalVariables->GetFloatValue(groupName, "generatedWidth_");

	blockSpped_[kBlockSpeedIndexFast] = globalVariables->GetFloatValue(groupName, "blockSpeed Fast");
	blockSpped_[kBlockSpeedIndexMiddle] = globalVariables->GetFloatValue(groupName, "blockSpeed Middle");
	blockSpped_[kBlockSpeedIndexSlow] = globalVariables->GetFloatValue(groupName, "blockSpeed Slow");

}
