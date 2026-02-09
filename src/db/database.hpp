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
	void Close();

private:
	sqlite3* db;
};
