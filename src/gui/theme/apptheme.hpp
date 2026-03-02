#pragma once
#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/wx.h>

struct AppTheme {
	static wxColor GetBgBlue() { return wxColor(0, 51, 153); }
	static wxColor GetTextWhite() { return *wxWHITE; }

	static wxFont GetSystemFont() {
		return wxFont(
			13,
			wxFONTFAMILY_DEFAULT,
			wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL,
			false,
			wxT("ＭＳ 明朝")
		);
	}

};
