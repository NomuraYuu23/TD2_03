#include "BlockManager.h"
#include "../Block/Block.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

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

	// ブロックの最大位置
	maxBlockPosition_ = {100.0f, 100.0f, 100.0f};

	// ブロックの最小位置
	minBlockPosition_ = { -100.0f, -100.0f, -100.0f };

	// ブロックの最大数
	maxBlock_ = 128;

	// ブロックのサイズ
	blockSize_[kBlockSizeIndexSmall] = { 1.0f, 1.0f, 1.0f };
	blockSize_[kBlockSizeIndexMiddle] = { 3.0f, 3.0f, 3.0f };
	blockSize_[kBlockSizeIndexBig] = { 5.0f, 5.0f, 5.0f };

	// ステート切り替え(ファクトリー)
	blockManagerFactory_ = BlockManagerFactory::GetInstance();

	// ステート(ブロックの生成パターン)
	blockManagerState_.reset(blockManagerFactory_->CreateBlockManagerState(kBlockManagerStateInit));
	blockManagerState_->Initialize();

	// 現在のステート番号
	currentStateNo_ = kBlockManagerStateInit;

	// 前のステート番号
	prevStateNo_ = kBlockManagerStateInit;

	// ブロック
	blocks_.clear();

	// モデル
	model_ = model;

	// マテリアル
	MaterialInitialize();

	// 外部変数登録
	RegisteringGlobalVariables();

	// 外部変数適用
	ApplyGlobalVariables();

}

void BlockManager::Update()
{

#ifdef _DEBUG

	// 外部変数適用
	ApplyGlobalVariables();

#endif // _DEBUG


	// 状態の変更
	StateChange();

	// 状態の更新
	blockManagerState_->Update();

	//ブロックの範囲制御
	RangeControl();

	// ブロックの数制御
	uint32_t maxGenerationBlock = NumberControl();

	// ブロックの生成
	blockManagerState_->GenerationBlock(maxGenerationBlock);

	// ブロックの更新
	for (Block* block : blocks_) {
		// ブロックの更新
		block->Update();
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

	Vector3 playerPosition = {0.0f, 0.0f, 0.0f}/*player_->GetWorldPosition()*/;

	blocks_.remove_if([=](Block* block) {
		
		// ブロックの位置取得
		Vector3 blockPosition = { 0.0f, 0.0f, 0.0f }/*block->GetWorldPositon()*/;
		
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

void BlockManager::StateChange()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	currentStateNo_ = blockManagerState_->GetBlockManagerStateNo();

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		blockManagerState_.reset(blockManagerFactory_->CreateBlockManagerState(currentStateNo_));
		blockManagerState_->Initialize();
	}

}

void BlockManager::GenerationBlock()
{

	Block* newBlock = new Block();
	newBlock->Initialize();
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

}
