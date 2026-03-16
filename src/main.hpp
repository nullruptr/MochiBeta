#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/string.h>
#include "core/db/database.hpp"

enum class ClockType {
	System,
	UTC
};

// フレーム初期化
class MBFrame: public wxFrame {
public:
	MBFrame(const wxString& title);
	void OnTimer(wxTimerEvent &event); // 時間表示をリアルタイムで行うために必要
	void OnQuit(wxCommandEvent &event); // 終了インベント処理
	void OnTimeLog(wxCommandEvent &event); // 時間記録画面を開く
	void OnConnectDB(wxCommandEvent &event);
	Database db;
	wxString current_DB_Path;
	void OnActivityReport(wxCommandEvent& event);
private:
	wxTimer timer;
	wxStaticText *sys_c;
	wxStaticText *UTC_c;
	wxFont clock_font;
};

wxString now(ClockType type);

