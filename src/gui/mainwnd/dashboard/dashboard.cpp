#include "dashboard.hpp"
#include <wx/datetime.h>
#include <wx/event.h>
#include <wx/wx.h>


Dashboard::Dashboard(wxWindow* parent, Database &dbRef) 
	: wxPanel(parent, wxID_ANY)
	  , m_db(dbRef) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	// --- 上部 ---
	wxBoxSizer* range_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* date_label = new wxStaticText(this, wxID_ANY, _("Range: "));
	m_date_range = new wxChoice(this, wxID_ANY);

	m_date_range->Append(_("Today"));
	m_date_range->Append(_("This Week"));
	m_date_range->Append(_("This Month"));
	m_date_range->Append(_("This Year"));
	m_date_range->Append(_("Last 1 Month"));
	m_date_range->Append(_("Last 3 Months"));
	m_date_range->Append(_("Last 6 Months"));
	m_date_range->Append(_("Last 1 Year"));
	m_date_range->Append(_("Last 3 Years"));
	m_date_range->Append(_("Custom"));

	// 最初の項目（Today）をデフォルトで選択状態にする
	if (!m_date_range->IsEmpty()) {
		m_date_range->SetSelection(0);
	}

	m_period_display = new wxStaticText(this, wxID_ANY, wxEmptyString);
	m_date_picker_start = new wxDatePickerCtrl(this, wxID_ANY);
	m_date_picker_end = new wxDatePickerCtrl(this, wxID_ANY);

	// 初期状態ではDatePickerを隠す
	m_date_picker_start->Hide();
	m_date_picker_end->Hide();

	m_btn_update = new wxButton(this, wxID_ANY, _("Update"));

	range_sizer->Add(date_label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	range_sizer->Add(m_date_range, 0, wxALIGN_CENTER_VERTICAL);
	range_sizer->Add(m_period_display, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	range_sizer->Add(m_date_picker_start, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	range_sizer->Add(m_date_picker_end, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	range_sizer->Add(m_btn_update, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	sizer->Add(range_sizer, 0, wxALL, 10);
	this->SetSizer(sizer);

	m_date_range->Bind(wxEVT_CHOICE, &Dashboard::OnRangeChanged, this);
	
	// 初回起動時に期間を表示させるためのダミーイベント
	wxCommandEvent dummy;
	OnRangeChanged(dummy);
  }

void Dashboard::OnRangeChanged(wxCommandEvent& event) {
	int selIdx = m_date_range->GetSelection();
	if (selIdx == wxNOT_FOUND) return;

	wxDateTime now = wxDateTime::Now();
	wxDateTime start = now, end = now;
	
	// 時刻を00:00:00にリセットして日付ベースの計算に
	start.ResetTime();
	end.SetHour(23).SetMinute(59).SetSecond(59);

	switch (selIdx) {
		case RANGE_TODAY:
			start = now; // (規定)
			break;

		case RANGE_THIS_WEEK:
			// 月曜日開始
			start = now.GetWeekDayInSameWeek(wxDateTime::Mon);
			break;

		case RANGE_THIS_MONTH:
			start.SetDay(1);
			break;

		case RANGE_THIS_YEAR:
			start.SetMonth(wxDateTime::Jan);
			start.SetDay(1);
			break;

		case RANGE_LAST_1_MONTH:
			start.Subtract(wxDateSpan(0, 1, 0, 0));
			break;

		case RANGE_LAST_3_MONTHS:
			start.Subtract(wxDateSpan(0, 3, 0, 0));
			break;

		case RANGE_LAST_6_MONTHS:
			start.Subtract(wxDateSpan(0, 6, 0, 0));
			break;

		case RANGE_LAST_1_YEAR:
			start.Subtract(wxDateSpan(1, 0, 0, 0));
			break;

		case RANGE_LAST_3_YEARS:
			start.Subtract(wxDateSpan(3, 0, 0, 0));
			break;

		case RANGE_CUSTOM:
			// Custom の場合は表示を切り替えて即リターン
			m_period_display->Hide();
			m_date_picker_start->Show();
			m_date_picker_end->Show();
			this->GetSizer()->Layout();
			return;

		default:
			return;
	}

	// Custom 以外の場合の共通処理
	m_date_picker_start->Hide();
	m_date_picker_end->Hide();
	m_period_display->Show();

	// 表示形式の整形 (例: 2026-04-06)
	m_period_display->SetLabel(start.FormatISODate() + " - " + end.FormatISODate());

	this->GetSizer()->Layout();
}
