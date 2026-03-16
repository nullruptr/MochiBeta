#include "activity_report.hpp"
#include <wx/wx.h>
#include <wx/dateevt.h>

ActivityReport::ActivityReport(wxWindow* parent, Database& dbRef)
    : wxFrame(parent, wxID_ANY, _("Activity Report"),
              wxDefaultPosition, wxSize(600, 400))
    , db(dbRef)
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // --- 上部: 日付ピッカー ---
    wxBoxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* date_label = new wxStaticText(panel, wxID_ANY, _("Date: "));
    m_datePicker = new wxDatePickerCtrl(panel, wxID_ANY, wxDefaultDateTime,
                                        wxDefaultPosition, wxDefaultSize,
                                        wxDP_DEFAULT | wxDP_SHOWCENTURY);
    top_sizer->Add(date_label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    top_sizer->Add(m_datePicker, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(top_sizer, 0, wxALL, 10);

    // --- 中部: リスト ---
    m_list = new wxListCtrl(panel, wxID_ANY,
                            wxDefaultPosition, wxDefaultSize,
                            wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
    m_list->InsertColumn(0, _("Category"), wxLIST_FORMAT_LEFT, 350);
    m_list->InsertColumn(1, _("Total Time"), wxLIST_FORMAT_LEFT, 150);
    sizer->Add(m_list, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    // --- 下部: 合計表示 ---
    m_total_label = new wxStaticText(panel, wxID_ANY, _("Total: 0h 0m 0s"));
    sizer->Add(m_total_label, 0, wxALL, 10);

    panel->SetSizer(sizer);

    // 日付変更イベント
    m_datePicker->Bind(wxEVT_DATE_CHANGED, &ActivityReport::OnDateChanged, this);

    CenterOnParent();
    LoadReport(); // 初期表示（今日）
}

void ActivityReport::OnDateChanged(wxDateEvent& event) {
    LoadReport();
}

void ActivityReport::LoadReport() {
    m_list->DeleteAllItems();

    wxDateTime local_dt = m_datePicker->GetValue();
    wxString local_date_str = local_dt.Format("%Y-%m-%d");
    int offset_seconds = wxDateTime::TimeZone(wxDateTime::Local).GetOffset();

    std::vector<Database::RecordSummary> summaries;
    if (!db.GetRecordsByDate(local_date_str.ToStdString(),
                             offset_seconds,
                             summaries)) {
        return;
    }

    int total_seconds = 0;
    for (const auto& rs : summaries) {
        wxString name = wxString::FromUTF8(rs.category_name.c_str());
        int h = rs.total_seconds / 3600;
        int m = (rs.total_seconds % 3600) / 60;
        int s = rs.total_seconds % 60;
        wxString time_str = wxString::Format("%dh %dm %ds", h, m, s);
        long idx = m_list->InsertItem(m_list->GetItemCount(), name);
        m_list->SetItem(idx, 1, time_str);
        total_seconds += rs.total_seconds;
    }

    int th = total_seconds / 3600;
    int tm_m = (total_seconds % 3600) / 60;
    int ts = total_seconds % 60;
    m_total_label->SetLabel(
        wxString::Format(_("Total: %dh %dm %ds"), th, tm_m, ts)
    );
}
