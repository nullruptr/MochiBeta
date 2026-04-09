#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include "core/db/database.hpp"

class CategoryTree : public wxTreeCtrl {
public:
	CategoryTree(wxWindow* parent, Database& db);
	void OnItemSelected(wxTreeEvent& event);
	void OnUpdateKeyDown(wxKeyEvent& event); // F5 で更新処理を受信
	void UpdateTreeData();
	void BuildTree(
			int parentId,
			wxTreeItemId parentNode,
			const std::vector<Database::Category> &categories
		      );
private:
	Database& m_db;
};
