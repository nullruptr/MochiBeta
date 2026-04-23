#include "dashboard.hpp"
#include "gui/mainwnd/mainwnd.hpp"
#include <string>
#include <wx/datetime.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/wx.h>


Dashboard::Dashboard(wxWindow* parent, Database &dbRef) 
	: wxPanel(parent, wxID_ANY)
	  , m_db(dbRef) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// --- info ---
	wxFlexGridSizer* info_grid = new wxFlexGridSizer(4, 2, 5, 5);

	wxStaticText* label_ID = new wxStaticText(this, wxID_ANY, _("ID: "));
	m_label_ID_num = new wxStaticText(this, wxID_ANY, _("None"));
	wxStaticText* label_cat_name = new wxStaticText(this, wxID_ANY, _("Category Name: "));
	m_label_cat_name_result = new wxStaticText(this, wxID_ANY, _("None"));
	wxStaticText* label_path = new wxStaticText(this, wxID_ANY, _("Path:"));
	m_label_path = new wxStaticText(this, wxID_ANY, _("None"));
	wxStaticText* label_record = new wxStaticText(this, wxID_ANY, _("Record"));
	m_btn_start = new wxButton(this, wxID_ANY, _("Start"));

	info_grid->Add(label_ID, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(m_label_ID_num, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(label_cat_name, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(m_label_cat_name_result, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(label_path, 0, wxALIGN_CENTER_VERTICAL);
	info_grid->Add(m_label_path, 0, wxALIGN_CENTER_VERTICAL);
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
	m_cb_auto_update = new wxCheckBox(this, wxID_ANY, _("Auto Update"));
	// デフォルトでチェックあり
	m_cb_auto_update->SetValue(true);

	range_sizer->Add(date_label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	range_sizer->Add(m_date_range, 0, wxALIGN_CENTER_VERTICAL);
	range_sizer->Add(m_period_display, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	range_sizer->Add(m_date_picker_start, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	range_sizer->Add(m_date_picker_end, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	range_sizer->Add(m_btn_update, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	range_sizer->Add(m_cb_auto_update, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
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
	
	wxStaticText* label_search_scope = new wxStaticText(this, wxID_ANY, _("Search Scope:"));
	m_btn_search_scope = new wxButton(this, wxID_ANY, _("Detail"));

	wxStaticText* stat_total_time_all = new wxStaticText(this, wxID_ANY, _("Total Time (All-time):"));
	m_result_total_time_all = new wxStaticText(this, wxID_ANY, _("00:00:00"));
	wxStaticText* stat_total_time_range = new wxStaticText(this, wxID_ANY, _("Total Time (Selected Range):"));
	m_result_total_time_range = new wxStaticText(this, wxID_ANY, _("00:00:00"));
	wxStaticText* stat_streak = new wxStaticText(this, wxID_ANY, _("Current Streak:"));
	wxStaticText* result_streak = new wxStaticText(this, wxID_ANY, _("0d"));
	wxStaticText* stat_last_run = new wxStaticText(this, wxID_ANY, _("Last Executed:"));
	wxStaticText* result_last_run = new wxStaticText(this, wxID_ANY, _("0000-00-00"));

	stat_grid->Add(label_search_scope, 0, wxALIGN_CENTER_VERTICAL, 5);
	stat_grid->Add(m_btn_search_scope, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_total_time_all, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(m_result_total_time_all, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_total_time_range, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(m_result_total_time_range, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_streak, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_streak, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(stat_last_run, 0, wxALIGN_CENTER_VERTICAL);
	stat_grid->Add(result_last_run, 0, wxALIGN_CENTER_VERTICAL);
	
	stat_box->Add(stat_grid, 0, wxLEFT, 10);
	sizer->Add(stat_box, 0, wxALL, 10);

	this->SetSizer(sizer);

	m_date_range->Bind(wxEVT_CHOICE, &Dashboard::OnRangeChanged, this);
	m_btn_start->Bind(wxEVT_BUTTON, &Dashboard::OnStartRecordEvtSend, this);
	// dashboard.cpp のコンストラクタ内
	// event 引数が2つ以上あるので、ラムダ式で対応
	m_btn_update->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		this->OnUpdateStatistics(event, EventType::FROM_MYSELF);
	});

	m_date_picker_start->Bind(wxEVT_DATE_CHANGED, [this](wxDateEvent& event) {
		this->OnUpdateStatistics(event, EventType::FROM_MYSELF);
	});

	m_date_picker_end->Bind(wxEVT_DATE_CHANGED, [this](wxDateEvent& event) {
		this->OnUpdateStatistics(event, EventType::FROM_MYSELF);
	});
	
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
	// end は「翌日の00:00:00」
	end = start + wxDateSpan(0, 0, 1, 0); // +1日

	switch (selIdx) {
		case RANGE_TODAY:
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
			// 安全のため初期化
			start.ResetTime();
			end.ResetTime();
			end = start + wxDateSpan(0, 0, 1, 0);
			break;

			default:
			return;
	}


	// 時刻情報をメンバ変数に渡す。Update でこれを利用する。
	m_current_start = start;
	m_current_end = end;


	// custom のとき
	if (selIdx == RANGE_CUSTOM) {
		start = m_date_picker_start->GetValue();
		end = m_date_picker_end->GetValue();

		start.ResetTime();
		end.ResetTime();
		end += wxDateSpan(0, 0, 1, 0);
	} else {
		// Custom 以外の場合の共通処理
		m_date_picker_start->Hide();
		m_date_picker_end->Hide();
		m_period_display->Show();
	}


	wxDateTime display_end = end - wxDateSpan(0, 0, 1, 0);
	// 表示形式の整形 (例: 2026-04-06)
	m_period_display->SetLabel(start.FormatISODate() + " - " + display_end.FormatISODate());

	// m_cb_auto_update が true のとき
	if (m_cb_auto_update->GetValue()) {
		wxCommandEvent evt_update;
		OnUpdateStatistics(evt_update, EventType::FROM_MYSELF);

		// activity report 更新イベントを投げる
		wxCommandEvent evt_activity_report(wxEVT_MENU, ID_ACTIVITY_REPORT);
		evt_activity_report.SetEventObject(this);
		wxPostEvent(GetParent(), evt_activity_report);
	}

	this->GetSizer()->Layout();
}

// 記録開始イベント送信
void Dashboard::OnStartRecordEvtSend(wxCommandEvent& event) {
	wxCommandEvent evt_start(wxEVT_MENU, ID_START_RECORDING);

	if (m_selected_id == -1) {
		wxMessageBox(_("Please select a category first."), _("Error"), wxOK | wxICON_ERROR);
		return;
	}
	// イベントの発生源を自分に設定
	evt_start.SetInt(m_selected_id);
	evt_start.SetEventObject(this);
	evt_start.SetString(m_label_cat_name_result->GetLabelText());
	// 親ウィンドウへ向かってイベントを投げる
	wxPostEvent(GetParent(), evt_start);
}

// 上部 info 更新
void Dashboard::UpdateSelectedCategory(int id, const wxString& name) {
	m_selected_id = id; // ほかに引き渡す用途
	m_label_ID_num->SetLabel(wxString::Format("%d", id));
	m_label_cat_name_result->SetLabel(name);

	// カテゴリのパス取得
	std::string path = m_db.GetCategoriesPath(id);
	if (path.empty()) {
		m_label_path->SetLabel(_("None"));
	} else {
		m_label_path->SetLabel(wxString::FromUTF8(path));
	}

	// m_cb_auto_update が true のとき
	if (m_cb_auto_update->GetValue()) {
		wxCommandEvent evt_update;
		OnUpdateStatistics(evt_update, EventType::FROM_MYSELF);
	}
	// レイアウト崩れ防止
	this->Layout();
}

void Dashboard::OnUpdateStatistics(wxCommandEvent& event, EventType type) {
	if (m_selected_id == -1) return;

	// RANGE_CUSTOM の場合、ボタンを押した瞬間の Picker の値を再取得する
	if (m_date_range->GetSelection() == RANGE_CUSTOM) {
		m_current_start = m_date_picker_start->GetValue();
		m_current_end = m_date_picker_end->GetValue();
		m_current_start.ResetTime();
		m_current_end += wxDateSpan(0, 0, 1, 0);
	}

	// 指定された時刻をDBに渡せるようstd::string へ
	std::string start_utc = m_current_start.ToUTC().Format("%Y-%m-%d %H:%M:%S").ToStdString();
	std::string end_utc = m_current_end.ToUTC().Format("%Y-%m-%d %H:%M:%S").ToStdString();
	// 全範囲の合計時間取得用
	std::string start_utc_all = "1970-01-01 00:00:00";

	// DBへ
	long long total_sec = m_db.GetTotalTime(m_selected_id, start_utc, end_utc);
	long long total_sec_all = m_db.GetTotalTime(m_selected_id, start_utc_all, end_utc);

	// 時間表示の整形
	auto format_time = [](long long total_seconds) -> wxString {
		long long h = total_seconds / 3600;
		long long m = (total_seconds % 3600) / 60;
		long long s = total_seconds % 60;
		return wxString::Format("%02lld:%02lld:%02lld", h, m, s);
	};

	// 表示
	// 分と秒は2桁固定
	m_result_total_time_range->SetLabel(wxString::Format(format_time(total_sec)));
	m_result_total_time_all->SetLabel(wxString::Format(format_time(total_sec_all)));

	// 表示内容更新イベント
	// 無限ループ防止のために、MAINWND から更新処理が来たら、除外する
	if (type == EventType::FROM_MYSELF) {
		wxCommandEvent evt(wxEVT_MENU, ID_UPDATE_STATISTICS);
		evt.SetEventObject(this);
		wxPostEvent(GetParent(), evt);
	}
}

// MainWnd から期間を取得するためのゲッタ
void Dashboard::GetCurrentRange(wxDateTime& start, wxDateTime& end) const {
	start = m_current_start;
	end = m_current_end;
}
