#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "core/db/database.hpp"

class ActivityReport : public wxFrame {
public:
    ActivityReport(wxWindow* parent, Database& dbRef);

private:
    void LoadReport();
    void OnDateChanged(wxDateEvent& event);

    Database& db;
    wxDatePickerCtrl* m_datePicker;
    wxListCtrl* m_list;
    wxStaticText* m_total_label;
};
