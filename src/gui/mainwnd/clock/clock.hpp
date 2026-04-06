#pragma once
#include <wx/wx.h>
#include "core/clock/now_wxstring.hpp"

class Clock : public wxPanel {
public:
	Clock(wxWindow* parent);
	void OnTimer(wxTimerEvent& event); 
private:
	wxStaticText* m_currentTime;
	wxTimer m_timer;
	Now_wxStr m_nowStr;
};

