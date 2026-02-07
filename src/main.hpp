#pragma once
#include <wx/string.h>

enum class ClockType {
	System,
	UTC
};

wxString now(ClockType type);
void OnTimeLog(); // 時間記録画面を開く
