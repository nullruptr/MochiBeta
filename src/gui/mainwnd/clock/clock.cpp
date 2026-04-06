#include "core/clock/now_wxstring.hpp"
#include "clock.hpp"
#include <wx/wx.h>

Clock::Clock(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	  , m_timer(this) {
		
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_currentTime = new wxStaticText(this, wxID_ANY, m_nowStr.now(ClockType::System));
	
	sizer->Add(m_currentTime, 0, wxALL | wxALIGN_CENTER, 5);
	SetSizer(sizer);

			// タイマイベント関連付け
	this->Bind(wxEVT_TIMER, &Clock::OnTimer, this); // タイマイベントが発生したら、この(this)イベントの OnTimerを呼び出す

    // 1秒(1000ms)間隔でスタート
    m_timer.Start(1000);
}

void Clock::OnTimer(wxTimerEvent& WXUNUSED(event)){
	m_currentTime->SetLabel(m_nowStr.now(ClockType::System));
}
