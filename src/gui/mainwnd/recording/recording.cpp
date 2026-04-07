#include "recording.hpp"
#include <wx/dataview.h>
#include <wx/wx.h>

Recording::Recording(wxWindow* parent) : wxPanel(parent) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_dvlc = new wxDataViewListCtrl(this, wxID_ANY);

	// 列の定義
	m_dvlc->AppendTextColumn(_("Category Name"), wxDATAVIEW_CELL_INERT, 150);
	m_dvlc->AppendTextColumn(_("Start Time"), wxDATAVIEW_CELL_EDITABLE, 150);
	m_dvlc->AppendTextColumn(_("Elapsed Time"), wxDATAVIEW_CELL_EDITABLE, 150);

	wxDataViewColumn* buttonCol = new wxDataViewColumn(
	    _("Action"), 
	    new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_ACTIVATABLE), 
	    3, 80, wxALIGN_CENTER
	);
	m_dvlc->AppendColumn(buttonCol);

	sizer->Add(m_dvlc, 1, wxEXPAND);
	SetSizer(sizer);
}
