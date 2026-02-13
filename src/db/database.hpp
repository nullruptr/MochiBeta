#pragma once
#include <sqlite3.h>
#include <string>

class Database {
public:
	Database();
	~Database();
	bool Connect(const std::string& path);
	bool Create(const std::string& path);
	bool Initialize();
	bool InsertCategories(const std::string &name, int parent_id);
	bool InsertRecords(const std::string &name, int parent_id);
	void Close();

private:
	sqlite3* db;
};
