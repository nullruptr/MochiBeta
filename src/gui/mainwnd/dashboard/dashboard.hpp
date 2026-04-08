#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/datectrl.h>
#include "core/db/database.hpp"

class Dashboard : public wxPanel {
public:
	Dashboard(wxWindow* parent, Database& db);
	void OnRangeChanged(wxCommandEvent& event);
private:
	Database &m_db;
	wxChoice *m_date_range;
	wxTextCtrl* m_text_cat_name;
	wxStaticText* m_period_display;
	wxButton *m_btn_save;
	wxDatePickerCtrl* m_date_picker_start;
	wxDatePickerCtrl* m_date_picker_end;
	wxButton *m_btn_start;
	wxButton *m_btn_update;
	wxButton *m_btn_offset_y_prev;
	wxButton *m_btn_offset_y_next;
	wxButton *m_btn_offset_m_prev;
	wxButton *m_btn_offset_m_next;
	wxButton *m_btn_offset_d_prev;
	wxButton *m_btn_offset_d_next;
};

enum RangeIndex {
	RANGE_TODAY = 0,
	RANGE_THIS_WEEK,
	RANGE_THIS_MONTH,
	RANGE_THIS_YEAR,
	RANGE_LAST_1_MONTH,
	RANGE_LAST_3_MONTHS,
	RANGE_LAST_6_MONTHS,
	RANGE_LAST_1_YEAR,
	RANGE_LAST_3_YEARS,
	RANGE_CUSTOM
};
