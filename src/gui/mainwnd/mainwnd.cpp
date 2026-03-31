#include <wx/wx.h>
#include <wx/aui/aui.h>
#include "mainwnd.hpp"
#include "gui/time_log/time_log.hpp"
#include "gui/connect_db/connect_db.hpp"
#include "gui/activity_report/activity_report.hpp"

Mainwnd::Mainwnd(wxWindow* parent) : wxFrame(parent, wxID_ANY, _("wxAUI Test"),
	wxDefaultPosition, wxSize(800,600),
	wxDEFAULT_FRAME_STYLE) {
        // notify wxAUI which frame to use
        m_mgr.SetManagedWindow(this);

	// メニュー内容の設定
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_CONNECT_DB, _("Connect DB"));
	menuFile->Append(ID_SETTINGS, _("Settings (Not implemented)"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, _("Quit\t F12"));

	wxMenu *menuLegacy = new wxMenu;
	menuLegacy->Append(ID_TIME_LOG, _("Time Log"));
	menuLegacy->Append(ID_ACTIVITY_REPORT, _("Activity Report"));

	// メニューバーの設定
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("File"));
	menuBar->Append(menuLegacy, _("Legacy"));
	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &Mainwnd::OnConnectDB, this, ID_CONNECT_DB);
	Bind(wxEVT_MENU, &Mainwnd::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &Mainwnd::OnTimeLog, this, ID_TIME_LOG);
	Bind(wxEVT_MENU, &Mainwnd::OnActivityReport, this, ID_ACTIVITY_REPORT);

	// テキストコントロールの作成
	wxTextCtrl* text1 = new wxTextCtrl(this, wxID_ANY, _("Pane 1"), 
			       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxTextCtrl* text2 = new wxTextCtrl(this, wxID_ANY, _("Pane 2"), 
			       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxTextCtrl* text3 = new wxTextCtrl(this, wxID_ANY, _("Main Content"), 
                                       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

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
	
        // add the panes to the manager
	m_mgr.AddPane(text1, wxAuiPaneInfo().Left().Caption(_("Pane Number One")));
	m_mgr.AddPane(text2, wxAuiPaneInfo().Bottom().Caption(_("Pane Number Two")));
	m_mgr.AddPane(text3, wxAuiPaneInfo().CenterPane());

        // tell the manager to "commit" all the changes just made
        m_mgr.Update();
    }

void Mainwnd::OnQuit(wxCommandEvent& WXUNUSED(event)){ // 終了確認
	int btn_status = wxMessageBox(_("Are you sure you want to quit?"),
			wxT("MB"),
			wxOK| wxCANCEL | wxICON_INFORMATION,
			this);

	if (btn_status != wxOK){ // OK以外が押されたら何も処理しない
		return;
	}

	Close(true);
}

void Mainwnd::OnTimeLog(wxCommandEvent& event){
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

void Mainwnd::OnConnectDB(wxCommandEvent& event){
	ConnectDB dlg(this); // 新規ダイアログ作成

	if (dlg.ShowModal() == wxID_OK){ // モーダル表示。ファイル確認完了後、接続処理へ。
		std::string path = std::string(dlg.GetPath().ToUTF8());

		if (db.Connect(path)){
			// ステータスバーに設定
			current_DB_Path = dlg.GetPath();
			SetStatusText(_("Connected"), 0);
			SetStatusText(current_DB_Path, 1);
		} else {
			wxMessageBox(_("Connection failed"));
		}
	}
}

void Mainwnd::OnActivityReport(wxCommandEvent& event) {
	if (current_DB_Path.IsEmpty()) {
		wxMessageBox(_("Unable to connect database"),
			     "DB Error", wxOK | wxICON_ERROR, this);
		return;
	}
	ActivityReport* report = new ActivityReport(this, db);
	report->Show(true);
}

Mainwnd::~Mainwnd(){
        // deinitialize the frame manager
        m_mgr.UnInit();
}
