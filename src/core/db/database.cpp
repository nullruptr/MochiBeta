#include "core/db/database.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>

Database::Database() : db(nullptr) {
	
}

Database::~Database(){ // 終了時処理。安全のため、デストラクタでもクローズ。
	Close();
}

bool Database::Connect(const std::string& path) { // DB に接続 
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

	if (db == nullptr){ // DB が開いていなかったら，抜ける
		return false;
	}

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


// --- stmt についてのメモ ---
//
// stmt は sqlite3_stmt* 型，&stmt は sqlite3_stmt** 型
// 関数側の仮引数を sqlite3_stmt** ppStmt とすると，呼び出し時に
// ppStmt == &stmt ...(1)
// となる．
// (1)に両辺に * を付け加えると，
// *ppStmt == *(&stmt)...(2)
// となる．
// ここで，&stmt は，stmt のアドレス，*(&stmt)は取得されたアドレスの中身を見るということから，
// *(&stmt) == stmt
// より，*(&stmt) は，stmt そのものとなる．
//
// 次に，temp をSQL構文を解析した結果を格納する構造体とする．
// SQLite 内部で，
// sqlite3_stmt* temp = malloc(...)
// とし，構造体を格納するメモリを確保する．
// temp には，確保されたメモリのアドレスが格納される．
// ここで，(2)より，
// *ppStmt == temp;
// とすると，stmt はSQL構文解析結果が書かれた temp のアドレスとなる．
//
// 従って，sqlite3_prepare_v2 以降の stmt は，temp のアドレスそのものである．
// 故に，stmt から temp のアドレスにアクセスすることができる．
//
// -------

bool Database::InsertCategories(const std::string &name, int parent_id){
	if (db == nullptr){ // DB が開いていなかったら，抜ける
		return false;
	}

	// ? はプレースホルダ（後で値を流し込む場所）
	// categories に VALUES (?, ?) を INSERT する定義文．
	const char* sql = "INSERT INTO categories (name, parent_id) VALUES (?, ?);";

	// "stmt" というポインタ変数をスタックに確保
	sqlite3_stmt* stmt = nullptr;

	// 1. SQL文の準備
	// sqlite3_prepare_v2 の引数
	// 	1. db 接続ハンドル
	// 	2. SQL 文
	// 	3. SQL 文字列バイト数．-1で終端nullまで
	// 	4. sqlite3_stmt のアドレス
	// 	5. 基本 nullptr．複数SQL連結時のみ使用
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Prepare Error: " << sqlite3_errmsg(db) << std::endl; // sqlite3_prepare_v2 のときのエラー内容取得は，sqlite3_errmsg(db) らしい
	return false;
	}

	// 2. 値のバインド（? の部分に実際の値を割り当てる）
	// 
	// sqlite3_bind_text の引数
	// 	1. sqlite3_prepare_v2 で生成されたステートメント
	// 	2. プレースホルダ番号．スタートは１から
	// 	3. バインドする文字列
	// 	4. 文字列長．-1で終端nullまで
	// 	5. 文字列メモリの扱い方．安全のため，基本的に SQLITE_TRANSIENT(SQLite側でコピー作成)を使用
	//
	// 1番目の ? に name をセット
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
	// 2番目の ? に parent_id をセット
	// sqlite3_bind_int の引数
	// 	1. 2. は上記と同じ
	// 	3. バインドする整数値
	sqlite3_bind_int(stmt, 2, parent_id);

	// 3. 実行
	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) { 
		std::cerr << "Execution Error: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	// 4. 後片付け
	sqlite3_finalize(stmt); // stmt 解法
	std::cout << "Category '" << name << "' inserted successfully." << std::endl;
	return true;
}

bool Database::InsertRecords(int category_id, const std::string &time_begin, const std::string &time_end){
	if (db == nullptr){ 
		return false;
	}

	const char* sql = "INSERT INTO records (category_id, time_begin, time_end) VALUES (?, ?, ?);";

	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
		std::cerr << "Prepare Error: " << sqlite3_errmsg(db) << std::endl; 
		return false;
	}


	sqlite3_bind_int(stmt, 1, category_id);
	sqlite3_bind_text(stmt, 2, time_begin.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, time_end.c_str(), -1, SQLITE_TRANSIENT);

	// 実行
	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) { 
		std::cerr << "Execution Error: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt); // stmt 解法
	return true;
}



void Database::Close() { // 閉じる
	if (db != nullptr) {
		sqlite3_close_v2(db);
		db = nullptr;
	}
}
