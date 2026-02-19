#pragma once
#include <sqlite3.h>
#include <string>

class Database {
public:
	Database();
	~Database(); // 安全のため，Close()を呼んでる．
	bool Connect(const std::string& path); // 接続．接続先パス
	bool Create(const std::string& path);// 新規作成．接続先パス
	bool Initialize(); // 初期化処理
	// --- 書き込み処理 ---
	// カテゴリ登録
	bool InsertCategories(const std::string &name, int parent_id); // カテゴリ名，親ID
	// 開始時刻と終了時刻
	bool InsertRecords(int category_id, const std::string &time_begin, const std::string &time_end); // カテゴリテーブルでのID，開始時刻，終了時刻
	// ------

	void Close(); // DB クローズ処理

private:
	sqlite3* db;
};
