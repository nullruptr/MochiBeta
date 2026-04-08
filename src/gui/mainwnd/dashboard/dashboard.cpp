#include "dashboard.hpp"
#include <wx/datetime.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/wx.h>


Dashboard::Dashboard(wxWindow* parent, Database &dbRef) 
	: wxPanel(parent, wxID_ANY)
	  , m_db(dbRef) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// --- info ---
	wxFlexGridSizer* info_grid = new wxFlexGridSizer(3, 2, 5, 5);

	wxStaticText* label_ID = new wxStaticText(this, wxID_ANY, _("ID: "));
	wxStaticText* label_ID_num = new wxStaticText(this, wxID_ANY, _("None"));
	wxStaticText* label_cat_name = new wxStaticText(this, wxID_ANY, _("Category Name: "));

	// FlexGridSizer に TextCtrl と Button を1マスに同時に格納する為にまとめるためのサイザ
	// 実質セル結合
	wxBoxSizer* cat_input_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_text_cat_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_btn_save = new wxButton(this, wxID_ANY, _("Save"));

	cat_input_sizer->Add(m_text_cat_name, 1, wxEXPAND | wxRIGHT, 5);
	cat_input_sizer->Add(m_btn_save, 0, wxALIGN_CENTER_VERTICAL);

	wxStaticText* label_record = new wxStaticText(this, wxID_ANY, _("Record"));
	m_btn_start = new wxButton(this, wxID_ANY, _("Start"));


	info_grid->Add(label_ID, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(label_ID_num, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(label_cat_name, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(cat_input_sizer, 1, wxEXPAND); // サブサイザーを2列目に入れる
	info_grid->Add(label_record, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(m_btn_start, 0, wxALIGN_CENTER_VERTICAL);
	
	sizer->Add(info_grid, 0, wxEXPAND | wxALL, 10);

	// --- レンジ ---
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

		// --- オフセット操作エリア ---
	// 3行（年・月・日）× 3列（ラベル・前へ・次へ）のグリッド
	wxFlexGridSizer* offset_grid = new wxFlexGridSizer(3, 3, 5, 5);

	// ボタンのサイズ規定（小さく設定）
	wxSize btn_size(50, 30);

	// --- 年 ---
	wxStaticText* label_y = new wxStaticText(this, wxID_ANY, _("Year: "));
	m_btn_offset_y_prev = new wxButton(this, wxID_ANY, "<", wxDefaultPosition, btn_size);
	m_btn_offset_y_next = new wxButton(this, wxID_ANY, ">", wxDefaultPosition, btn_size);

	// --- 月 ---
	wxStaticText* label_m = new wxStaticText(this, wxID_ANY, _("Month: "));
	m_btn_offset_m_prev = new wxButton(this, wxID_ANY, "<", wxDefaultPosition, btn_size);
	m_btn_offset_m_next = new wxButton(this, wxID_ANY, ">", wxDefaultPosition, btn_size);

	// --- 日 ---
	wxStaticText* label_d = new wxStaticText(this, wxID_ANY, _("Day: "));
	m_btn_offset_d_prev = new wxButton(this, wxID_ANY, "<", wxDefaultPosition, btn_size);
	m_btn_offset_d_next = new wxButton(this, wxID_ANY, ">", wxDefaultPosition, btn_size);

	// グリッドに追加（順番に埋まっていく）
	offset_grid->Add(label_y, 0, wxALIGN_CENTER_VERTICAL);
	offset_grid->Add(m_btn_offset_y_prev);
	offset_grid->Add(m_btn_offset_y_next);

	offset_grid->Add(label_m, 0, wxALIGN_CENTER_VERTICAL);
	offset_grid->Add(m_btn_offset_m_prev);
	offset_grid->Add(m_btn_offset_m_next);

	offset_grid->Add(label_d, 0, wxALIGN_CENTER_VERTICAL);
	offset_grid->Add(m_btn_offset_d_prev);
	offset_grid->Add(m_btn_offset_d_next);

	sizer->Add(offset_grid, 0, wxALL, 10);

	// --- ステータス表示 ---
	wxStaticBoxSizer* stat_box = new wxStaticBoxSizer(wxVERTICAL, this, _("Statistics"));
	wxFlexGridSizer* stat_grid = new wxFlexGridSizer(5, 2, 8, 20);
	
	wxStaticText* stat_total_time = new wxStaticText(this, wxID_ANY, _("Total Time:"));
	wxStaticText* result_total_time = new wxStaticText(this, wxID_ANY, _("0h 0m 0s"));
	wxStaticText* stat_streak = new wxStaticText(this, wxID_ANY, _("Current Streak:"));
	wxStaticText* result_streak = new wxStaticText(this, wxID_ANY, _("0d"));
	wxStaticText* stat_last_run = new wxStaticText(this, wxID_ANY, _("Last Executed:"));
	wxStaticText* result_last_run = new wxStaticText(this, wxID_ANY, _("0000-00-00"));
	wxStaticText* stat_days_since = new wxStaticText(this, wxID_ANY, _("Days Since Last Run:"));
	wxStaticText* result_days_since = new wxStaticText(this, wxID_ANY, _("0d"));
	wxStaticText* stat_daily_average = new wxStaticText(this, wxID_ANY, _("Daily Average:"));
	wxStaticText* result_daily_average = new wxStaticText(this, wxID_ANY, _("0h 0m 0s"));

	stat_grid->Add(stat_total_time, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_total_time, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_streak, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_streak, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_last_run, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_last_run, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_days_since, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_days_since, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_daily_average, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_daily_average, 0, wxALIGN_CENTER_VERTICAL);
	
	stat_box->Add(stat_grid, 0, wxLEFT, 10);
	sizer->Add(stat_box, 0, wxALL, 10);

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
