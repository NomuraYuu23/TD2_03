#include "BlockPatternFile.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>

BlockPatternFile* BlockPatternFile::GetInstance()
{
	static BlockPatternFile instance;
	return &instance;
}

void BlockPatternFile::Update()
{

#ifdef _DEBUG

	if (!ImGui::Begin("BlockPatternFile", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
		++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.begin();
			itItem != group.end(); ++itItem) {

			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			uint32_t count = 0u;

			for (Item::iterator value = item.begin();
				value != item.end(); ++value) {
				if (ImGui::TreeNode(std::to_string(count).c_str())) {

					ImGui::DragFloat3("position", &value->position_.x);
					ImGui::DragFloat3("velocity", &value->velocity_.x);

					ImGui::TreePop();

				}
				count++;
			}
		}

		//改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			std::string message = std::format("Do you want to save the {} GlobalVariables file?", groupName);
			int ans = MessageBoxA(nullptr, message.c_str(), "Save confirmation",
				MB_ICONQUESTION | MB_OKCANCEL);
			if (ans == 1) {
				SaveFile(groupName);
				message = std::format("{}.json saved.", groupName);
				MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			}
		}

		ImGui::EndMenu();

	}

	ImGui::EndMenuBar();
	ImGui::End();

#endif // _DEBUG

}

void BlockPatternFile::LoadFiles()
{

	// 保存先ディレクトリのパスをローカル変数で宣言する
	std::filesystem::path dir(kDirectoryPath);
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(dir)) {
		return;
	}

	//各ファイルの処理
	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());

	}

}

void BlockPatternFile::LoadFile(const std::string& groupName)
{

	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に聞く
	ifs.open(filePath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// float型のjson配列登録
		std::vector<BlockPatternData> blockPatternDatas;
		
		for (nlohmann::json::iterator itItemIndex = itItem->begin(); itItemIndex != itItem->end(); ++itItemIndex) {
			BlockPatternData blockPatternData = itItemIndex->get<BlockPatternData>();
			blockPatternDatas.push_back(blockPatternData);
		}

		SetValue(groupName, itemName, blockPatternDatas);

	}

}

void BlockPatternFile::SetValue(const std::string& groupName, const std::string& key, const std::vector<BlockPatternData>& value)
{

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void BlockPatternFile::SaveFile(const std::string& groupName)
{

	//グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	//未登録チェック
	assert(itGroup != datas_.end());

	//グループ登録確認
	nlohmann::json root;

	root = nlohmann::json::object();

	// jsonオブジェクト登録
	root[groupName] = nlohmann::json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem) {

		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		root[groupName][itemName] = item;

		//ディレクトリがなければ作成する
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}

		// 書き込むJSONファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + groupName + ".json";
		// 書き込む用ファイルストリーム
		std::ofstream ofs;
		// ファイルを書き込み用に開く
		ofs.open(filePath);
		// ファイルオープン失敗
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		// ファイルにjson文字列を書き込む(インデント幅4)
		ofs << std::setw(4) << root << std::endl;
		// ファイルを閉じる
		ofs.close();

	}

}

void BlockPatternFile::CreateGroup(const std::string& groupName)
{

	//指定名のオブジェクトがなければ追加する
	datas_[groupName];

}

void BlockPatternFile::AddItem(const std::string& groupName, const std::string& key, const std::vector<BlockPatternData>& value)
{

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

std::vector<BlockPatternData> BlockPatternFile::GetValue(const std::string& groupName, const std::string& key)
{

	assert(datas_.find(groupName) != datas_.end());
	//グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return group[key];

}
