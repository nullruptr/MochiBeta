#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>

class Recording : public wxPanel {
public:
	Recording(wxWindow* parent);
private:
	wxDataViewListCtrl* m_dvlc;
};

