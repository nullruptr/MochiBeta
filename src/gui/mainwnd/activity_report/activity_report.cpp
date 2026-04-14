#include "activity_report.hpp"
#include <wx/wx.h>
#include <wx/dateevt.h>

ActivityReport::ActivityReport(wxWindow* parent, Database& dbRef)
    : wxPanel(parent, wxID_ANY)
    , m_db(dbRef)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // --- 中部: リスト ---
    m_list = new wxListCtrl(this, wxID_ANY,
                            wxDefaultPosition, wxDefaultSize,
                            wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
    m_list->InsertColumn(0, _("Category"), wxLIST_FORMAT_LEFT, 350);
    m_list->InsertColumn(1, _("Total Time"), wxLIST_FORMAT_LEFT, 150);
    sizer->Add(m_list, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    // --- 下部: 合計表示 ---
    m_total_label = new wxStaticText(this, wxID_ANY, _("Total: 0h 0m 0s"));
    sizer->Add(m_total_label, 0, wxALL, 10);

    this->SetSizer(sizer);


    CenterOnParent();
}


void ActivityReport::LoadReport() {
	m_list->DeleteAllItems();
	if (!m_current_start.IsValid()) return; //安全のため

	std::string s_utc = m_current_start.ToUTC().Format("%Y-%m-%d %H:%M:%S").ToStdString();
	std::string e_utc = m_current_end.ToUTC().Format("%Y-%m-%d %H:%M:%S").ToStdString();

	std::vector<Database::RecordSummary> summaries = m_db.GetRecordsByRange(s_utc, e_utc);

	long long total_seconds = 0;
	for (const auto& rs : summaries) {
		int h = rs.total_seconds / 3600;
		int m = (rs.total_seconds % 3600) / 60;
		int s = rs.total_seconds % 60;
        
		long idx = m_list->InsertItem(m_list->GetItemCount(), wxString::FromUTF8(rs.category_name));
		m_list->SetItem(idx, 1, wxString::Format("%dh %dm %ds", h, m, s));
		
		total_seconds += rs.total_seconds;
	}

	m_total_label->SetLabel(wxString::Format(_("Total: %lldh %lldm %llds"), 
        total_seconds / 3600, (total_seconds % 3600) / 60, total_seconds % 60));
}

void ActivityReport::SetPeriodAndRefresh(const wxDateTime& start, const wxDateTime& end) {
	m_current_start = start;
	m_current_end = end;

	// 再計算と表示更新
	LoadReport();
}
