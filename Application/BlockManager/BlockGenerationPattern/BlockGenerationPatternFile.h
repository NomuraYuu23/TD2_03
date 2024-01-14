#pragma once
#include <vector>
#include <map>
#include <string>
#include "BlockGenerationPatternData.h"

class BlockGenerationPatternFile
{

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static BlockGenerationPatternFile* GetInstance();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="partName">パーツ名</param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const std::vector<BlockGenerationPatternData>& value);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 項目の追加
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const std::vector<BlockGenerationPatternData>& value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">キー</param>
	/// <returns>値</returns>
	std::vector<BlockGenerationPatternData> GetValue(const std::string& groupName, const std::string& key);

private:
	BlockGenerationPatternFile() = default;
	~BlockGenerationPatternFile() = default;
	BlockGenerationPatternFile(const BlockGenerationPatternFile&) = delete;
	const BlockGenerationPatternFile& operator=(const BlockGenerationPatternFile&) = delete;
	
	//項目

	using Item = std::vector<BlockGenerationPatternData>;
	using Group = std::map<std::string, Item>; // パターン名

	// データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/BlockPatternFile/";

};

