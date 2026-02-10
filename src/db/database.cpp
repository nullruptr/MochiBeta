#include "database.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>

Database::Database() : db(nullptr) {
	
}

Database::~Database(){ // 終了時処理。安全のため、デストラクタでもクローズ。
	Close();
}

bool Database::Connect(const std::string& path) { // DB を開く
	int rc = sqlite3_open_v2(
			path.c_str(),
			&db,
			SQLITE_OPEN_READWRITE, // 開くだけ
			nullptr);

	if (rc != SQLITE_OK){ // SQLite が、開かなかった場合の処理
		std::cerr << sqlite3_errmsg(db) << std::endl;
		db = nullptr;
		return false;
	}

	return true;
}

bool Database::Create(const std::string& path) { // DB 新規作成
	int rc = sqlite3_open_v2(
			path.c_str(), // C++ -> C の char に変換
			&db,
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, // 開いてみて、なかったら新規作成する
			nullptr);

	if (rc != SQLITE_OK){ // SQLite が、開かなかった場合の処理
		db = nullptr;
		return false;
	}

	return true;
}

bool Database::Initialize(){
	std::cout << "Starting Initialize..." << std::endl;

	const char* sql =
		"CREATE TABLE IF NOT EXISTS categories ("// 同名のテーブルが存在しない場合のみ新しいテーブルを作成し、既存の場合はエラーを出さずに処理をスキップ（無視）する
		"id INTEGER PRIMARY KEY AUTOINCREMENT," // 整数、主キー、連番(INSERT時に id を書く必要がなくなる)
		"parent_id INTEGER," // 親子関係を識別する
		"name TEXT NOT NULL" // カテゴリ名。TEXT->文字列型、NOT NULL -> 空欄禁止（何かしら値を入れる）
		")"
		";"

		"CREATE TABLE IF NOT EXISTS records ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"category_id INTEGER NOT NULL," // カテゴリ番号。 categories の id と同じ番号に相当する
		"time_begin TEXT NOT NULL," // 開始時刻
		"time_end TEXT NOT NULL" // 終了時刻
		")"
		";";

	char *errMsg = nullptr;
	int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg); // 書き込み処理

	if (rc != SQLITE_OK) {
		std::cerr << "SQL Error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		return false;
	}
		
	return true;	
}

bool Database::Insert(){
	return true;
}



void Database::Close() { // 閉じる
	if (db != nullptr) {
		sqlite3_close(db);
		db = nullptr;
	}
}
