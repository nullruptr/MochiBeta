#include <wx/msgdlg.h>
#include <wx/wx.h>
#include "main.hpp"
#include "core/db/database.hpp"
#include "gui/time_log/time_log.hpp"
#include "gui/connect_db/connect_db.hpp"

enum {
	ID_CONNECT_DB, //DBファイル
	ID_SETTINGS, //設定ウィンドウ
	ID_TIME_LOG, //時間記録画面
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
private:
	wxTimer timer;
	wxStaticText *sys_c;
	wxStaticText *UTC_c;
	wxFont clock_font;
};


void MBFrame::OnTimer(wxTimerEvent &event){ // タイマイベントによって、周期的に呼び出される。
	sys_c->SetLabel(now(ClockType::System)); // SetLabel で、wxStaticTextの中身を書き換える。
	UTC_c->SetLabel(now(ClockType::UTC));
}

// フレーム内容記述
// エントリから受け取ったタイトルを記述、
// ウィンドウサイズ指定、
// タイマを使うので、呼び出し、
// 時刻フォント設定
MBFrame::MBFrame(const wxString& title) : 
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1024, 768)),
	timer(this),
	clock_font( // フォントサイズ (pt)、フォントファミリ、フォントスタイル（通常）、太さ（通常、下線なし）、フォント名
			40,
			wxFONTFAMILY_DEFAULT,
			wxFONTSTYLE_NORMAL,
			wxFONTWEIGHT_NORMAL,
			false,
			"Noto Sans JP"
			)
	{ 
		
		wxPanel *FLS_panel = new wxPanel(this, wxID_ANY); // パネル作成
		wxBoxSizer *FLS_sizer = new wxBoxSizer(wxVERTICAL); // サイザの準備
		
		wxGridSizer *FLS_grid_sizer = new wxGridSizer(2, 1, 20, 20); // 
		FLS_sizer->Add(FLS_grid_sizer, 1, wxEXPAND); // 設定したサイザにグリッドしたものを適応
		FLS_panel->SetSizer(FLS_sizer);
		sys_c = new wxStaticText(FLS_panel, wxID_ANY, now(ClockType::System)); // 表示先はメインパネルを設定
		UTC_c = new wxStaticText(FLS_panel, wxID_ANY, now(ClockType::UTC)); 
		FLS_grid_sizer->Add(sys_c, 0, wxALIGN_CENTER); // 日付を呼ぶ
		FLS_grid_sizer->Add(UTC_c, 0, wxALIGN_CENTER); // 日付を呼ぶ

		sys_c->SetFont(clock_font); // 時刻のフォント設定
		UTC_c->SetFont(clock_font); 

		// タイマイベント関連付け
		Bind(wxEVT_TIMER, &MBFrame::OnTimer, this); // タイマイベントが発生したら、この(this)イベントの OnTimerを呼び出す

		timer.Start(50); // 50 ms 周期でwxEVIT_TIMERを呼び出し

		// メニュー内容の設定
		wxMenu* menuFile = new wxMenu;
		menuFile->Append(ID_CONNECT_DB, _("Connect DB"));
		menuFile->Append(ID_SETTINGS, _("Settings (Not implemented)"));
		menuFile->AppendSeparator();
		menuFile->Append(wxID_EXIT, _("Quit\t F12"));

		wxMenu *menuFunctions = new wxMenu;
		menuFunctions->Append(ID_TIME_LOG, _("Time Log"));
		menuFunctions->AppendSeparator();
		menuFunctions->Append(wxID_ANY, _("Template (Not implemented)"));
		menuFunctions->Append(wxID_ANY, _("Project (Not implemented)"));

		wxMenu *menuManagement = new wxMenu;
		menuManagement->Append(wxID_ANY, _("Edit Category (Not implemented)"));
		menuManagement->Append(wxID_ANY, _("Activity Report (Not implemented)"));

		// メニューバーの設定
		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuFile, _("File"));
		menuBar->Append(menuFunctions, _("Functions"));
		menuBar->Append(menuManagement, _("Management"));
		SetMenuBar(menuBar);

		Bind(wxEVT_MENU, &MBFrame::OnConnectDB, this, ID_CONNECT_DB); // 接続用ウィンドウ
		Bind(wxEVT_MENU, &MBFrame::OnQuit, this, wxID_EXIT); // 終了ボタン -> 終了イベント
		Bind(wxEVT_MENU, &MBFrame::OnTimeLog, this, ID_TIME_LOG);


		// ステータスバーの設定
		CreateStatusBar(2);
		// 初期表示
		SetStatusText(_("Disconnected"), 0);
		SetStatusText("", 1);

		// F12 で終了処理
		wxAcceleratorEntry entry;
		entry.Set(wxACCEL_NORMAL, WXK_F12, wxID_EXIT);
		SetAcceleratorTable(wxAcceleratorTable(1, &entry));

		CenterOnScreen(); // 画面真ん中に表示
	}

// 現在時刻を表示する
// ex. 2026-01-01 23:04:59
//
// GUI として表示させるため、wxString形式で扱う。
wxString now(ClockType  type){  
	int yy, mm, dd, day, wd, hour, min, sec;

	wxDateTime dt;
	// 引数にsys が入れば、システム時刻を、それ以外はUTCを返す。
	if (type == ClockType::System){
		dt = wxDateTime::Now();
	}
	else {
		dt = wxDateTime::Now().ToUTC();
	}

	yy = dt.GetYear();
	mm = dt.GetMonth() + 1; // 月は+1表示する必要がある。cf. 0は1月のこと
	day = dt.GetDay();
	wd = dt.GetWeekDay();
	hour = dt.GetHour();
	min = dt.GetMinute();
	sec = dt.GetSecond();

	// return するときは、この形式で返す必要アリ。文字列で返す。printfのような使い方
	return wxString::Format(
		"%04d-%02d-%02d %02d:%02d:%02d",
		yy, mm, day, hour, min, sec
			);
}

void MBFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){ // 終了確認
	int btn_status = wxMessageBox(_("Are you sure you want to quit?"),
			wxT("MB"),
			wxOK| wxCANCEL | wxICON_INFORMATION,
			this);

	if (btn_status != wxOK){ // OK以外が押されたら何も処理しない
		return;
	}

	Close(true);
}

void MBFrame::OnTimeLog(wxCommandEvent& event){
	// DB 接続判定
	if (current_DB_Path.IsEmpty()){
		wxMessageBox(
				_("Unable to connect database"),
				"DB Error",
				wxOK | wxICON_ERROR,
				this
			    );
		return;
	}
	TimeLog* log = new TimeLog(this, db, current_DB_Path);
	log->Show(true);
}

void MBFrame::OnConnectDB(wxCommandEvent& event){
	ConnectDB dlg(this); // 新規ダイアログ作成

	if (dlg.ShowModal() == wxID_OK){ // モーダル表示。ファイル確認完了後、接続処理へ。
		std::string path = dlg.GetPath().ToStdString();

		if (db.Connect(path)){
			wxMessageBox(_("Connected"));
			// ステータスバーに設定
			current_DB_Path = dlg.GetPath();
			SetStatusText(_("Connected"), 0);
			SetStatusText(current_DB_Path, 1);
		} else {
			wxMessageBox(_("Connection failed"));
		}

	}

}




class MBApp : public wxApp {
public:
	bool OnInit() override;
private:
	wxLocale locale;
};

wxIMPLEMENT_APP(MBApp);

// エントリ
bool MBApp::OnInit() {
	locale.Init(wxLANGUAGE_ENGLISH); // 使用言語設定
	locale.AddCatalogLookupPathPrefix("locale");
	locale.AddCatalog("mb");
	MBFrame *mainFrame = new MBFrame("MochiBeta"); // Build habits the sticky way
	mainFrame->Show(true); 
	return true;
}

