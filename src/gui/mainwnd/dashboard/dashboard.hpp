#pragma once
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "core/db/database.hpp"

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

// OnUpdateStatistics において、イベントがどこから来たかによって制御を変えるため
enum class EventType {
	FROM_MAINWND,
	FROM_MYSELF
};

class Dashboard : public wxPanel {
public:
	Dashboard(wxWindow* parent, Database& db);
	void OnRangeChanged(wxCommandEvent& event);
	void OnStartRecordEvtSend(wxCommandEvent& event);
	void UpdateSelectedCategory(int id, const wxString& name);
	void OnUpdateStatistics(wxCommandEvent& event, EventType type);
	void GetCurrentRange(wxDateTime& start, wxDateTime& end) const;
private:
	Database &m_db;
	wxStaticText* m_label_ID_num;
	wxChoice *m_date_range;
	wxTextCtrl* m_text_cat_name;
	wxStaticText* m_period_display;
	wxButton *m_btn_save;
	wxDatePickerCtrl* m_date_picker_start;
	wxDatePickerCtrl* m_date_picker_end;
	wxButton *m_btn_start;
	wxButton *m_btn_update;
	wxCheckBox* m_cb_auto_update;
	wxButton *m_btn_offset_y_prev;
	wxButton *m_btn_offset_y_next;
	wxButton *m_btn_offset_m_prev;
	wxButton *m_btn_offset_m_next;
	wxButton *m_btn_offset_d_prev;
	wxButton *m_btn_offset_d_next;
	wxStaticText* m_result_total_time;
	wxDateTime m_current_start;
	wxDateTime m_current_end;

	int m_selected_id = -1; // 受け取ったIDを int で格納しておく
};

