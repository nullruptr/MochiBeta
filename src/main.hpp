#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/string.h>

// フレーム初期化
class MBFrame: public wxFrame {
public:
	void OnTimer(wxTimerEvent &event); // 時間表示をリアルタイムで行うために必要
private:
	wxTimer timer;
	wxStaticText *sys_c;
	wxStaticText *UTC_c;
	wxFont clock_font;
};

