// database の接続テストです．CLI 専用です．wxWidgets 側では使いません．
#include <cstdio>
#include <iostream>
#include <string>
#include <chrono>
#include "core/db/database.hpp"

void init(const std::string& path);
void connect(const std::string& path);
void inscat(const std::string& path);
void insrec(const std::string& path);
void insrec(const std::string& path);

int main(){
	std::string path;
	std::cout << "DB テスト" << std::endl;
	std::cout << "DB のパスを入力してください。" << std::endl;
	std::cin >> path;
	
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
			init(path);
			break;
		case 2:
			std::cout << "DB 接続テストを開始します。" << std::endl;
			connect(path);
			break;
		case 3:
			std::cout << "カテゴリ登録テストを開始します．" << std::endl;
			inscat(path);
			break;
		case 4:
			std::cout << "レコードテストを開始します．" << std::endl;
			insrec(path);
			break;

	}
	return 0;
}

void init(const std::string& path){

	Database db; // インスタンス作成
	
	if(db.Connect(path)){
		std::cout << "オープン成功" << std::endl;
	} else {
		std::cout << "オープン失敗" << std::endl;
		std::cout << "新規作成します" << std::endl;

		if (db.Create(path)){ // なかったら作る
			std::cout << "作成成功" << std::endl;
			std::cout << "ファイル名: " << path << std::endl;
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

void connect(const std::string& path){
	Database db; // インスタンス作成
	
	if (db.Connect(path)) {
		std::cout << "DB接続成功" << std::endl;
	} else {
		std::cout << "DB接続失敗" << std::endl;
	}
	db.Close(); // 閉じる
}


void inscat(const std::string& path){
	Database db;
	if (!db.Connect(path)){ // 接続
		std::cout << "DB接続失敗" << std::endl;
		return;
	}

	std::string name;
	int parent_id;

	std::cout << "カテゴリ名称: ";
	std::cin >> name;

	std::cout << "親のID(なければ0): ";
	std::cin >> parent_id;

	if (db.InsertCategories(name, parent_id)){
		std::cout << "登録成功" << std::endl;
	} else {
		std::cout << "登録失敗" << std::endl;
	}

	db.Close();

}


void insrec(const std::string& path){
	Database db;
	if (!db.Connect(path)){ // 接続
		std::cout << "DB接続失敗" << std::endl;
		return;
	}

	int category_id;
	std::string time_begin;
	std::string time_end;

	std::cout << "カテゴリID: ";
	std::cin >> category_id;

	std::cout << "開始時刻: ";
	
	getchar();

	std::cout << "終了時刻: ";
	if (db.InsertRecords(category_id, time_begin, time_end)){
		std::cout << "登録成功" << std::endl;
	} else {
		std::cout << "登録失敗" << std::endl;
	}

	db.Close();
}
