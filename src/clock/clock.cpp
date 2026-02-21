#include "db/clock.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>


std::string Clock::now_utc_iso8601(){
	auto now = std::chrono::system_clock::now(); // 現在時刻を取得。time_point 型。
	std::time_t t = std::chrono::system_clock::to_time_t(now); // now を time_t 型へ
	std::tm tm = *std::gmtime(&t); // time_t -> UTCの std::tm 構造体へ (gmtime はUTC)
	std::ostringstream oss; // 文字出力先
	oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ"); // 出力内容
	return oss.str(); // 出力されたものをstd::string へ
}
