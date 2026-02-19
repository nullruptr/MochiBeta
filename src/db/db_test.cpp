// database の接続テストです．CLI 専用です．wxWidgets 側では使いません．
#include <iostream>
#include <string>
#include "db/database.hpp"

void init();
void connect();
void inscat();
void insrec();

int main(){
	std::cout << "DB テスト" << std::endl;
	std::cout << "実行するテストを選択してください" << std::endl;

	int testcase;
	std::cout << "1." << "DB初期化テスト" << std::endl;
	std::cout << "2." << "DB接続テスト" << std::endl;
	std::cout << "3." << "カテゴリ登録テスト" << std::endl;
	std::cout << "4." << "レコードテスト" << std::endl;
	std::cin >> testcase;

	switch (testcase) {
		case 1:
			std::cout << "DB 初期化テストを開始します。" << std::endl;
			init();
			break;
		case 2:
			std::cout << "DB 接続テストを開始します。" << std::endl;
			connect();
			break;
		case 3:
			std::cout << "カテゴリ登録テストを開始します．" << std::endl;
			inscat();
			break;
		case 4:
			std::cout << "レコードテストを開始します．" << std::endl;
			insrec();
			break;

	}
	return 0;
}

void init(){

	Database db; // インスタンス作成
	std::string db_path = "test_db.db";
	
	if(db.Connect(db_path)){
		std::cout << "オープン成功" << std::endl;
	} else {
		std::cout << "オープン失敗" << std::endl;
		std::cout << "新規作成します" << std::endl;

		if (db.Create(db_path)){ // なかったら作る
			std::cout << "作成成功" << std::endl;
			std::cout << "ファイル名: " << db_path << std::endl;
		} else {
			std::cerr << "DB作成失敗" << std::endl;
		}

		// テーブル初期化のテスト
		if (db.Initialize()) {
			std::cout << "テーブルの初期化に成功しました。" << std::endl;
		}
	
		db.Close(); // 閉じる
	}

}

void connect(){
	Database db; // インスタンス作成
	std::string db_path = "test_db.db";
	
	if (db.Connect(db_path)) {
		std::cout << "DB接続成功" << std::endl;
	} else {
		std::cout << "DB接続失敗" << std::endl;
	}
	db.Close(); // 閉じる
}


void inscat(){
	Database db;
	std::string db_path = "test_db.db";

	std::string name;
	int parent_id;

	std::cout << "カテゴリ名称: ";
	std::cin >> name;
	std::cout << std::endl;

	std::cout << "親のID: ";
	std::cin >> parent_id;


}
void insrec(){
	Database db;
	std::string db_path = "test_db.db";
}
