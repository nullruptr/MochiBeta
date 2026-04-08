#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>
#include "mainwnd.hpp"
#include "gui/mainwnd/dashboard/dashboard.hpp"
#include "gui/time_log/time_log.hpp"
#include "gui/connect_db/connect_db.hpp"
#include "gui/activity_report/activity_report.hpp"
#include "gui/mainwnd/treectrl/treectrl.hpp"

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

	Bind(wxEVT_COMMAND_MENU_SELECTED, &Mainwnd::OnCategorySelected, this, ID_CATEGORY_SELECTED);

	// Dashboard
	m_dashboard = new Dashboard(this, db);
	// Clock パネルの生成
	m_clock = new Clock(this);
	// Recording
	m_recording = new Recording(this);
	// TreeCtrl
	m_categoryTree = new CategoryTree(this, db);

	m_mgr.AddPane(m_dashboard, wxAuiPaneInfo().CenterPane());

	m_mgr.AddPane(m_clock, wxAuiPaneInfo()
	    .Name(wxT("clockPane"))      // 内部識別名
	    .Caption(_("Clock"))         // キャプション
	    .Top()                       // 画面上部に配置
	    .Layer(1)                    // レイヤー設定（他のペインとの前後関係）
	    .PaneBorder(false)           // 境界線を消す
	    .CaptionVisible(false)       // 時計にタイトルバーは不要なので false
	    .CloseButton(false)          // 閉じられないように設定
	    .DockFixed()                 // 位置を固定したい場合
	    .BestSize(-1, 40)            // 高さを 40px 程度に固定
	);
	
	m_mgr.AddPane(m_recording, wxAuiPaneInfo()
        .Bottom()
        .Caption(_("Recording"))
        .Name(wxT("Recording_wnd"))
        .BestSize(250, 250)
        .Layer(0)
	.Position(0)
	.PaneBorder(true)
	.CloseButton(false) // 閉じるボタン無効
	); 

	m_mgr.AddPane(m_categoryTree, wxAuiPaneInfo()
        .Left()
        .Caption(_("Categories"))
        .Name(wxT("treePane"))
        .BestSize(250, -1)
        .Layer(1)
	.CloseButton(false) // 閉じるボタン無効
	); 

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
			m_categoryTree->UpdateTreeData();
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

void Mainwnd::OnCategorySelected(wxCommandEvent& event) {
	// イベントに格納された数値を引っ張り出す
	int catId = event.GetInt();
	wxString catName = event.GetString();

	// Dashboard の表示を更新
	if (m_dashboard) {
		m_dashboard->UpdateSelectedCategory(catId, catName);
	}
}

Mainwnd::~Mainwnd(){
        // deinitialize the frame manager
        m_mgr.UnInit();
}
