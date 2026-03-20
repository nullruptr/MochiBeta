#pragma once
#include "gui/connect_db/connect_db.hpp"
#include "core/db/database.hpp"
#include "core/clock/clock.hpp"
#include <wx/event.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/propgrid.h>
#include <wx/string.h>
#include <wx/treebase.h>
#include <wx/wx.h>
#include <wx/treectrl.h>

class TimeLog : public wxFrame{

public:
	TimeLog(wxWindow* parent, Database &dbRef, const wxString& dbPath);
	void OnCreateNewCategory(wxCommandEvent& event);
	void OnStartRecord(wxCommandEvent& event); // レコード開始ウィンドウ遷移ボタン
	void OnSetTreeCtrlItem(wxCommandEvent &event);
	void OnItemSelected(wxTreeEvent& event); // TreeCtrl の項目が選択されたときに呼び出す
	void OnDeleteItem(wxCommandEvent& event); // イベント削除
	void LoadCategories();
	void BuildTree(
			int parentId,
			wxTreeItemId parentNode,
			const std::vector<Database::Category> &categories
		      );
	void OnQuit(wxCommandEvent &event); // 終了インベント処理

private:
	// tree をメンバ変数にする
	wxTreeCtrl* m_tree;
	wxButton *btn_record;
	wxButton *btn_end;

	// ツリーで右クリックしたときの処理
	void OnTreeRightClick(wxTreeEvent& event);
	// 編集メニューの設定
	void OnEditItem(wxCommandEvent& event);
	wxString m_dbPath;
	ConnectDB cdb;
	Database& db;
	wxStaticText* category_value;
	Clock cl;
	int m_selectedCategoryId; // 選択されたカテゴリID
	std::string startTime; // 開始時刻
	std::string endTime; // 終了時刻
	
	// プロパティグリッドの設定
	wxPropertyGrid* m_pg;
	wxPGProperty* m_propDbPath;
	wxPGProperty* m_propCategory;
};
