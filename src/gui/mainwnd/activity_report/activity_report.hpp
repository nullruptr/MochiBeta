#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "core/db/database.hpp"

class ActivityReport : public wxPanel {
public:
    ActivityReport(wxWindow* parent, Database& dbRef);
    void SetPeriodAndRefresh(const wxDateTime& start, const wxDateTime& end);

private:
	void LoadReport();

	Database& m_db;
	wxListCtrl* m_list;
	wxStaticText* m_total_label;
	wxDateTime m_current_start;
	wxDateTime m_current_end;
};
