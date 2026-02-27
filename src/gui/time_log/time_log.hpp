#pragma once
#include "gui/connect_db/connect_db.hpp"
#include "core/db/database.hpp"
#include <wx/string.h>
#include <wx/wx.h>
#include <wx/treectrl.h>

class TimeLog : public wxFrame{

public:
	TimeLog(wxWindow* parent, const wxString& dbPath);
	void OnCreateNewCategory(wxCommandEvent& event);
	void OnRecordStart(wxCommandEvent& event); // レコード開始ウィンドウ遷移ボタン
	void OnSetTreeCtrlItem(wxCommandEvent &event);
	void OnSaveCategory(wxCommandEvent &event);
	void OnQuit(wxCommandEvent &event); // 終了インベント処理

private:
	// tree をメンバ変数にする
	wxTreeCtrl* m_tree;
	wxTextCtrl *m_categoryText; // ツリーに表示された名称を編集するためのCtrl
	wxButton *btn_record;
	wxButton *btn_save;
	wxButton *btn_end;

	// ツリーで右クリックしたときの処理
	void OnTreeRightClick(wxTreeEvent& event);
	// 編集メニューの設定
	void OnEditItem(wxCommandEvent& event);
	wxString m_dbPath;
	ConnectDB cdb;
	Database db;
};
