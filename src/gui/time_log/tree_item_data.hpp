#pragma once
#include <wx/treebase.h>
#include <wx/wx.h>

class TreeItemData : public wxTreeItemData {
public:
	TreeItemData(int id) : m_id(id) {}
	int GetId() const { return m_id; }
private:
	int m_id;
};
