#pragma once
#include "wx/event.h"
#include "wx/treebase.h"
#include <wx/wx.h>
#include <wx/treectrl.h>

class TimeLog : public wxFrame{

public:
	TimeLog(wxWindow* parent);
	void OnCategoryDetail(wxCommandEvent& event); // カテゴリ詳細表示
	void OnRecordStart(wxCommandEvent& event); // レコード開始ウィンドウ遷移ボタン
	void OnQuit(wxCommandEvent &event); // 終了インベント処理

private:
	// tree をメンバ変数にする
	wxTreeCtrl* m_tree;

	// ツリーで右クリックしたときの処理
	void OnTreeRightClick(wxTreeEvent& event);
	// 編集メニューの設定
	void OnEditItem(wxCommandEvent& event);
};
