#pragma once
#include <wx/wx.h>

enum class ClockType {
	System,
	UTC
};

class Now_wxStr {
public:
	wxString now(ClockType type);
private:
};
