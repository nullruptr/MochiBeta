#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
public:
	Database();
	~Database(); // 安全のため，Close()を呼んでる．
	bool Connect(const std::string& path); // 接続．
	bool Create(const std::string& path);// 新規作成．接続先パス
	bool Initialize(); // 初期化処理
	// --- 書き込み ---
	bool InsertCategories(int parent_id, const std::string &name); // カテゴリ名，親ID
	bool InsertRecords(int category_id, const std::string &time_begin, const std::string &time_end); // カテゴリテーブルでのID，開始時刻，終了時刻
	struct Category{
		int id;
		int parent_id;
		std::string name;
	};
	bool GetAllCategories(std::vector<Category>& out);
	bool UpdateCategories(int id, const std::string& name);
	// ------


	void Close(); // DB クローズ処理

private:
	sqlite3* db;
	std::string path;
};
