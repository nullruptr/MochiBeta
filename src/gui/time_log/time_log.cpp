#include <vector>
#include <wx/stringimpl.h>
#include <wx/treebase.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include "time_log.hpp"
#include "edit_category.hpp"
#include "tree_item_data.hpp"
#include "core/db/database.hpp"
#include "gui/connect_db/connect_db.hpp"
#include "gui/record/record_window.hpp"

enum{
	ID_TREE_EDIT,
	ID_CREATE
};

TimeLog::TimeLog(wxWindow* parent, Database &dbRef, const wxString& dbPath)
	:wxFrame(parent, wxID_ANY, wxT("TimeLog Window"), wxDefaultPosition, wxSize())
	, cdb(this)
	, db(dbRef)
	
{
	m_dbPath = dbPath;
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

	wxTreeItemId root = tree->AddRoot("root");

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

	// パス表示用エリア
	
	wxBoxSizer* pathBox = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* path_st = new wxStaticText(pnl_time_log, wxID_ANY, _("DB Path: ") + m_dbPath); // パス表示
	path_st->SetFont(font);
	path_st->SetForegroundColour(*wxWHITE);
	path_st->SetBackgroundColour(wxColour(0, 51, 153));

	pathBox->Add(path_st, 0, wxRIGHT | wxEXPAND, 8);
	
	// カテゴリ表示用エリア
	wxStaticText* category_st = new wxStaticText(pnl_time_log, wxID_ANY, _("Category Name: ")); // カテゴリ StaticText

	wxBoxSizer *categorybox = new wxBoxSizer(wxHORIZONTAL);
	category_st->SetFont(font);
	category_st->SetForegroundColour(*wxWHITE);
	category_st->SetBackgroundColour(wxColour(0, 51, 153));

	categorybox->Add(category_st, 0, wxRIGHT | wxEXPAND, 8);

	timelog_sizer->Add(pathBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 5);
	timelog_sizer->Add(categorybox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 5);
	timelog_sizer->AddStretchSpacer(); // 中央に余白を追加

	// 下部の設定
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL); // 下部ボタン用サイザ
	

	btn_record = new wxButton(
			pnl_time_log,
			wxID_ANY,
			wxT("記録開始")
			);
	btn_end = new wxButton(
			pnl_time_log,
			wxID_EXIT,
			wxT("終了(F12)")
			);
	bottomSizer->AddStretchSpacer(); // 下部に余白を追加
	bottomSizer->Add(btn_record, 0, wxRIGHT | wxBOTTOM, 10); // レコードボタンをサイザへ登録
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

	LoadCategories(); // DB からカテゴリロード
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

	menu.Append(ID_CREATE, _("Create New Category"));
	menu.Append(wxID_EDIT, _("Edit"));
	menu.Append(wxID_ANY, _("Delete"));


	Bind(
		wxEVT_MENU,
		&TimeLog::OnCreateNewCategory,
		this,
		ID_CREATE	
	);
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

void TimeLog::OnCreateNewCategory(wxCommandEvent &event){
	// 現在選択されているアイテムを取得
	wxTreeItemId parent = m_tree->GetSelection();

	// 無効な場合は処理しない
	if (!parent.IsOk()){
		return;
	}

	int parent_db_id = 0;
	if (parent.IsOk()){
		TreeItemData* parentData = (TreeItemData*)m_tree->GetItemData(parent);
		if (parentData) { // データがあるとき(nullptr出ないとき)
			parent_db_id = parentData->GetId();
		}
		// parentData が nullptr (rootノード)なら、parent_db_id = 0 のまま
	}
	// 新規ノード名
	wxString newCategoryName = wxT("New Category");


	// 編集用画面呼び出し
	EditCategory dlg(this,
			newCategoryName,
			db,
			parent_db_id,
			0 // 新規のため、編集ID なし
			); // 編集用ダイアログ呼び出し
	if (dlg.ShowModal() == wxID_OK) {
		// 保存成功 -> ツリー再読み込み
		LoadCategories();
	}
	
}

void TimeLog::OnEditItem(wxCommandEvent& event){
	// ツリーでクリックされた内容を取得
	wxTreeItemId item = m_tree->GetFocusedItem();

	if (!item.IsOk()){
		return;
	}

	TreeItemData* data = (TreeItemData*)m_tree->GetItemData(item);

	if(!data){
		return;
	}

	int id = data->GetId();
	wxString currentName = m_tree->GetItemText(item);

	EditCategory dlg(this,
			currentName,
			db,
			0, // parentId は編集時不要
			id // editId
			);

	if (dlg.ShowModal() == wxID_OK) {
		LoadCategories(); // ツリー再読み込み
	}
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

void TimeLog::OnSetTreeCtrlItem(wxCommandEvent& event){
	wxString current_DB_Path = cdb.GetPath();
}


void TimeLog::LoadCategories(){
	std::vector<Database::Category> categories;

	if (!db.GetAllCategories(categories)){ // 全件格納。無理ならreturn。
		return;
	}

	m_tree->DeleteAllItems(); // ツリーリセット

	wxTreeItemId root = m_tree->AddRoot("root"); // 起点rootを作成
	BuildTree(0, root, categories);
	m_tree->ExpandAll();

	
}

void TimeLog::BuildTree(
		int parentId,
		wxTreeItemId parentNode,
		const std::vector<Database::Category>& categories
		) {
	for (const auto& c : categories){ // カテゴリ数だけループ
		if (c.parent_id == parentId){ // DB のparent_id と 引数の数値が同値の時
			wxTreeItemId node = m_tree->AppendItem(parentNode, c.name, -1, -1, new TreeItemData(c.id)); // 親の場所に子の名前でツリー登録。同時にID(c.id) をデータとして紐付け。
			BuildTree(c.id, node, categories); // 再起処理。自分のIDとノードを渡して、カテゴリ全件も渡す。子が存在したらぶら下げる。
		}
	}
}


void TimeLog::OnQuit(wxCommandEvent& WXUNUSED(event)){ // 終了
	Close(true);
}
