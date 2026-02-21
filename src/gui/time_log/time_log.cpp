#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include "time_log.hpp"
#include "gui/record/record_window.hpp"

enum{
	ID_TREE_EDIT
};

TimeLog::TimeLog(wxWindow* parent)
	:wxFrame(parent, wxID_ANY, wxT("TimeLog Window"), wxDefaultPosition, wxSize())
{
	wxFont font(
		13,
		wxFONTFAMILY_DEFAULT,
		wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL,
		false,
		wxT("ＭＳ 明朝")
	);
	
	wxBoxSizer* sizermain = new wxBoxSizer(wxVERTICAL); // メインサイザ
	wxSplitterWindow* splittermain = new wxSplitterWindow(this, wxID_ANY); // 分割作成
	sizermain->Add(splittermain, 1,wxEXPAND, 0); // 分割をメインサイザへ追加

	// パネル設定
	wxPanel* pnl_treectrl = new wxPanel(splittermain, wxID_ANY);
	wxPanel* pnl_time_log = new wxPanel(splittermain, wxID_ANY);

	// --- begin TreeCtrl ---
	wxTreeCtrl* tree = new wxTreeCtrl(
		pnl_treectrl,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT
	);
	tree->SetBackgroundColour(wxColour(0, 51, 153));
	tree->SetForegroundColour(wxColour(255, 255, 255));

	tree->SetFont(font);

	wxTreeItemId root = tree->AddRoot(wxT("業務"));

	wxTreeItemId cat1 = tree->AppendItem(root, wxT("開発"));
	tree->AppendItem(cat1, wxT("設計"));
	tree->AppendItem(cat1, wxT("実装"));
	tree->AppendItem(cat1, wxT("レビュー"));

	wxTreeItemId cat2 = tree->AppendItem(root, wxT("会議"));
	tree->AppendItem(cat2, wxT("定例"));
	tree->AppendItem(cat2, wxT("打合せ"));

	tree->ExpandAll();

	wxBoxSizer* treeSizer = new wxBoxSizer(wxVERTICAL);
	treeSizer->Add(tree, 1, wxEXPAND);
	pnl_treectrl->SetSizer(treeSizer);

	// ---ツリーを右クリックした時の設定---
	// 関数外でも使えるように、tree をメンバ変数m_tree とした。m_tree はヘッダ記載
	m_tree = tree;
	// ツリー内で右クリックされると、関数を呼び出すようにする
	m_tree->Bind(
			wxEVT_TREE_ITEM_RIGHT_CLICK,
			&TimeLog::OnTreeRightClick,
			this
		    );
	// end settings
	
	// --- end TreeCtrl --- 

	// --- begin time_log ---
	// 背景色設定
	pnl_time_log->SetBackgroundColour(wxColour(192, 192, 192));

	wxBoxSizer *timelog_sizer = new wxBoxSizer(wxVERTICAL); // サイザ

	wxBoxSizer *categorybox = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* category_st = new wxStaticText(pnl_time_log, wxID_ANY, wxT("区分: ")); // カテゴリ StaticText
	category_st->SetFont(font);
	category_st->SetForegroundColour(*wxWHITE);
	category_st->SetBackgroundColour(wxColour(0, 51, 153));
	categorybox->Add(category_st, 1, wxRIGHT | wxEXPAND, 8);
	timelog_sizer->Add(categorybox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	wxButton* btn1 = new wxButton(pnl_time_log, wxID_ANY, wxT("ボタン1"));
	timelog_sizer->Add(btn1, 0, wxEXPAND | wxALL, 5);

	timelog_sizer->AddStretchSpacer(); // 中央に余白を追加

	// 下部の設定
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL); // 下部ボタン用サイザ
	

	wxButton* btn_record = new wxButton(
			pnl_time_log,
			wxID_ANY,
			wxT("記録開始")
			);
	wxButton* btn_end = new wxButton(
			pnl_time_log,
			wxID_EXIT,
			wxT("終了(F12)")
			);
	bottomSizer->Add(btn_record, 0, wxLEFT | wxBOTTOM, 10); // レコードボタンをサイザへ登録
	bottomSizer->AddStretchSpacer(); // 下部に余白を追加
	bottomSizer->Add(btn_end, 0, wxRIGHT | wxBOTTOM, 10); // 終了ボタンをサイザへ登録
	
	timelog_sizer->Add(bottomSizer, 0, wxEXPAND); // 下部サイザを時間記録用サイザへ登録
	
	pnl_time_log->SetSizer(timelog_sizer); // サイザをパネルに登録
	
	// 記録開始ボタンのBind
	btn_record->Bind(wxEVT_BUTTON, &TimeLog::OnRecordStart, this); // 記録開始ボタン -> レコード開始ボタン遷移

	// F12 でウィンドウを閉じる
	wxAcceleratorEntry entry;
	entry.Set(wxACCEL_NORMAL, WXK_F12, wxID_EXIT);
	SetAcceleratorTable(wxAcceleratorTable(1, &entry));

	// 終了ボタンでウィンドウを閉じる
	btn_end->Bind(wxEVT_BUTTON, &TimeLog::OnQuit, this); // 終了ボタン -> 終了イベント
	// --- end time_log ---

	// --- about splitter Settigs ---
	splittermain->SplitVertically(pnl_treectrl, pnl_time_log); // パネルを左右分割スピリッタに登録

	// ペイン設定 
	splittermain->SetSashPosition(250); // 起動時にpx固定
	splittermain->SetSashGravity(0.0);// Sash を左に固定
	splittermain->SetMinimumPaneSize(250); // 最小サイズ指定

	// Fit は使わない
	SetSizer(sizermain);
	SetSize(wxSize(900, 600));
	CenterOnParent(); // 親ウィンドウの真ん中に表示する
}

void TimeLog::OnTreeRightClick(wxTreeEvent& event){
	// イベント内容取得
	wxTreeItemId item = event.GetItem();

	// 右クリックしても Item がなかったら処理しない
	if (!item.IsOk()){
		return;
	}

	// クリックによって取得されたアイテムを選択状態にする。
	m_tree->SelectItem(item);

	// 右クリックメニューの作成
	wxMenu menu;

	menu.Append(wxID_EDIT, _("Edit"));


	//wxID_MENUのメニューにおいて、wxID_EDITが呼ばれたら、TimeLog::OnEditItemを呼び出すよう指示。
	//
	Bind(
		wxEVT_MENU,
		&TimeLog::OnEditItem,
		this,
		wxID_EDIT
	);

	// マウスカーソル位置にメニューを表示する
	PopupMenu(&menu);
	

}

void TimeLog::OnEditItem(wxCommandEvent& event){
	// ツリーでクリックされた内容を取得
	wxTreeItemId item = m_tree->GetFocusedItem();

	// 右クリックしても Item がなかったら処理しない
	if (!item.IsOk()){
		return;
	}

	// 選択されたアイテムの文字列を取得
	wxString text = m_tree->GetItemText(item);

		wxMessageBox(
		wxT("編集対象: ") + text,
		_("Edit"),
		wxOK | wxICON_INFORMATION,
		this
	);
}

void TimeLog::OnCategoryDetail(wxCommandEvent& event){
	
}

void TimeLog::OnRecordStart(wxCommandEvent& event){ // レコード開始ウィンドウ遷移処理
	
	this->Hide(); // 親ウィンドウを隠す
	
	RecordWindow* rec_wnd = new RecordWindow(this); // record_window 生成
	
	// RecordWindow の終了処理をフック。
	// wxEVT_CLOSE_WINDOW (Alt + F4 や、xボタン)の発火を感知したら、自分の関数に持ち込んで処理する。
	// ラムダ式の[this]は、一見コピーキャプチャに見えるが、thisはポインタなので、this オブジェクトをコピーしているわけではない。
	rec_wnd->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event){

		this->Show(); // 隠していたものを再表示
		this->Raise(); // ウィンドウを最上位へ表示
		event.Skip(); // デフォルトの wxFrame クローズ処理に処理を委譲する
	}
	);
	
	rec_wnd->Show(); // 生成したものを表示

}


void TimeLog::OnQuit(wxCommandEvent& WXUNUSED(event)){ // 終了
	Close(true);
}
