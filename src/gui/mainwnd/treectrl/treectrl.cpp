#include <wx/treebase.h>
#include <wx/wx.h>
#include "gui/mainwnd/mainwnd.hpp"
#include "treectrl.hpp"
#include "gui/time_log/tree_item_data.hpp"
#include "core/db/database.hpp"

CategoryTree::CategoryTree(wxWindow* parent, Database &dbRef) 
	: wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT) ,
	m_db(dbRef) 
	{
		AddRoot(_("No Database Connected"));
		Bind(wxEVT_TREE_SEL_CHANGED, &CategoryTree::OnItemSelected, this);
}

void CategoryTree::OnItemSelected(wxTreeEvent& event) {
	// クリックされたツリーの項目のwxTreeItemId を取得
	wxTreeItemId item = event.GetItem();
	if (!item.IsOk()) return;
	
	// 紐づけたツリーのID を取得
	TreeItemData* data = static_cast<TreeItemData*>(GetItemData(item));
	if (data) {
		wxCommandEvent evt(wxEVT_MENU, ID_CATEGORY_SELECTED);
		evt.SetInt(data->GetId());            // カテゴリIDを格納
		evt.SetString(GetItemText(item));     // カテゴリ名を格納
	        // イベントの発生源を自分に設定
		evt.SetEventObject(this);
		// 親ウィンドウへ向かってイベントを投げる
		GetParent()->GetEventHandler()->ProcessEvent(evt);
	}
}

void CategoryTree::UpdateTreeData() {

	std::vector<Database::Category> categories;

	if (!m_db.GetAllCategories(categories)){ // 全件格納。無理ならreturn。
		return;
	}

	DeleteAllItems(); // ツリーリセット

	wxTreeItemId root = AddRoot("root"); // 起点rootを作成
	BuildTree(0, root, categories);
	Expand(root);
}

void CategoryTree::BuildTree(
		int parentId,
		wxTreeItemId parentNode,
		const std::vector<Database::Category>& categories
		) {
	for (const auto& c : categories){ // カテゴリ数だけループ
		if (c.parent_id == parentId){ // DB のparent_id と 引数の数値が同値の時
			wxString displayName = wxString::FromUTF8(c.name.c_str()); // UTF8 で読み込み
			wxTreeItemId node = AppendItem(parentNode, displayName, -1, -1, new TreeItemData(c.id)); // 親の場所に子の名前でツリー登録。同時にID(c.id) をデータとして紐付け。
			BuildTree(c.id, node, categories); // 再起処理。自分のIDとノードを渡して、カテゴリ全件も渡す。子が存在したらぶら下げる。
		}
	}
}

