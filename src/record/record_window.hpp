#pragma once
#include <wx/wx.h>

class RecordWindow : public wxFrame {
public:
	RecordWindow(wxWindow* parent);
	~RecordWindow(); // 終了時処理
	void OnTimer(wxTimerEvent& event);
private:
	wxTimer r_timer;
	int yy = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0; // Add these to store the time components

	wxStaticText* timerText;
};
