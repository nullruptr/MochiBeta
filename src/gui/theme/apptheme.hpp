#pragma once
#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/settings.h>
#include <wx/wx.h>

struct AppTheme {
	static wxColor GetBgBlue() { return wxColor(0, 51, 153); }
	static wxColor GetLightBlue() { return wxColor(85, 255, 255); }
	static wxColor GetTextWhite() { return *wxWHITE; }

	static wxFont GetSystemFont() {
		wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
		font.SetPointSize(11);
		return font;
	}
};
