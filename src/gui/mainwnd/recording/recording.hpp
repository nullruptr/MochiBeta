#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/dataview.h>
#include "core/db/database.hpp"

class Recording : public wxPanel {
public:
	Recording(wxWindow* parent, Database& db);
	void OnStartRecord(int id, const wxString& name);
	void OnStopRecord(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnSelectionChanged(wxDataViewEvent& event);
private:
	Database& m_db;
	
	int m_selected_id = -1; // 受け取った数値 int ver

	wxDataViewListCtrl* m_dvlc;
	wxButton* m_btn_stop;
	wxStaticText* m_st_rid;
	wxTimer m_timer{this};

};



