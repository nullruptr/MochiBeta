#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include "core/db/database.hpp"

class Todo : public wxFrame{

public:
	Todo(wxWindow* parent, Database& db);
private:
	Database &m_db;
	wxDataViewListCtrl* m_dvlc;
	wxButton* m_btn_pending;
	wxButton* m_btn_done;
	wxButton* m_btn_all;
	wxButton* m_btn_add;
	wxButton* m_btn_edit;
};
