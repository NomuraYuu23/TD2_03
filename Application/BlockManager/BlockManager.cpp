#include "BlockManager.h"

BlockManager::~BlockManager()
{

	for (Block* block : blocks_) {
		delete block;
	}

	blocks_.remove(nullptr);

}

void BlockManager::Initialize()
{

	// ブロックの最大位置
	maxBlockPosition_ = {100.0f, 100.0f, 100.0f};

	// ブロックの最大数
	maxBlock_ = 128;

	// ステート切り替え(ファクトリー)
	blockManagerFactory_ = BlockManagerFactory::GetInstance();

	// ステート(ブロックの生成パターン)
	blockManagerState_.reset(blockManagerFactory_->CreateBlockManagerState(0));

	// ブロック
	blocks_.clear();

}

void BlockManager::Update()
{

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
	}

}

void BlockManager::Draw()
{

	for (Block* block : blocks_) {
		// ブロックの描画
	}

}

void BlockManager::ImGuiDraw()
{
}

void BlockManager::RangeControl()
{

	blocks_.remove_if([=](Block* block) {
		
		// ブロックの位置取得
		Vector3 blockPosition = {0.0f,0.0f,0.0f};
		
		// ブロックが範囲を超えていたら削除
		if (blockPosition.x > maxBlockPosition_.x ||
			blockPosition.y > maxBlockPosition_.y ||
			blockPosition.z > maxBlockPosition_.z) {
			delete block;
			return true;
		}
		
		return false;
		
		});

}

uint32_t BlockManager::NumberControl()
{

	// 現在のブロックの数
	uint32_t blockNum = blocks_.size();

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
