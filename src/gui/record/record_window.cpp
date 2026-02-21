#include <wx/wx.h>
#include "record_window.hpp"

RecordWindow::RecordWindow(wxWindow* parent)
	: wxFrame(
		parent,                // 親ウィンドウ
		wxID_ANY,
		wxT("Recording..."),
		wxDefaultPosition,
		wxSize(700, 250),
		wxDEFAULT_FRAME_STYLE
	)
{
	wxFont clock_font( // フォントサイズ (pt)、フォントファミリ、フォントスタイル（通常）、太さ（通常、下線なし）、フォント名
			40,
			wxFONTFAMILY_DEFAULT,
			wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL,
			false,
			"Noto Sans JP"
			);

	wxPanel* record_wnd_panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* record_wnd_sizer = new wxBoxSizer(wxVERTICAL);

	r_timer.Start(1000); // 1s ごとにタイマイベントが発生するように設定
	r_timer.Bind(wxEVT_TIMER, &RecordWindow::OnTimer, this);

	// 先に0だけ埋めておく。あとからSetLabel で置換する
	timerText = new wxStaticText(record_wnd_panel, wxID_ANY, wxT("0000-00 00:00:00"));

	timerText->SetFont(clock_font); // フォント設定


	// サイザで中央に配置
	record_wnd_sizer->AddStretchSpacer(); // 上の余白
	record_wnd_sizer->Add(timerText, 0, wxALIGN_CENTER);
	record_wnd_sizer->AddStretchSpacer(); // 下の余白

	record_wnd_panel->SetSizer(record_wnd_sizer);

	// なぜかこれを入れないと0秒の時の表示がおかしい
	timerText->SetLabel(wxT("0000-00 00:00:00"));
	// レイアウトを強制更新
	this->Layout();

	CenterOnParent();
}

// タイマを止めるデストラクタ
// これがないとアプリケーションがクラッシュする
RecordWindow::~RecordWindow() {
    if (r_timer.IsRunning()) {
        r_timer.Stop();
	
    }
}

void RecordWindow::OnTimer(wxTimerEvent& event) {

	sec++;
	if (sec >= 60) {
		sec = 0;
		min++;
		if (min >= 60) {
			min = 0;
			hour++;
			if (hour >= 24) {
				hour = 0;
				day++;
				if (day >= 365) {
					day = 0;
					yy++;

				}
			}
		}
	}

	
	wxString timeStr = wxString::Format(
		"%04d-%02d %02d:%02d:%02d",
		yy, day, hour, min, sec
			);

	// 画面の更新
	timerText->SetLabel(timeStr);
	
}

