#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/treebase.h>
#include <wx/treectrl.h>
#include "core/db/database.hpp"
#include "gui/mainwnd/treectrl/treectrl.hpp"
#include "gui/mainwnd/clock/clock.hpp"

enum {
	ID_CONNECT_DB, //DBファイル
	ID_SETTINGS, //設定ウィンドウ
	ID_TIME_LOG, //時間記録画面
	ID_ACTIVITY_REPORT // 実績照会ウィンドウ表示
};

class Mainwnd : public wxFrame {
public:
Mainwnd(wxWindow* parent);
	void OnTimer(wxTimerEvent &event); // 時間表示をリアルタイムで行うために必要
	void OnQuit(wxCommandEvent &event); // 終了インベント処理
	void OnTimeLog(wxCommandEvent &event); // 時間記録画面を開く
	void OnConnectDB(wxCommandEvent &event);
	Database db;
	wxString current_DB_Path;
	void OnActivityReport(wxCommandEvent& event);

	
	
virtual ~Mainwnd();
private:
	wxAuiManager m_mgr;
	CategoryTree* m_categoryTree;
	Clock* m_clock;
};
