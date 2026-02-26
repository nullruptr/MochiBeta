#pragma once
#include <wx/event.h>
#include <wx/wx.h>

class ConnectDB : public wxDialog {
public:
	ConnectDB(wxWindow* parent);
	void OnBrowse(wxCommandEvent &event);
	void OnConnect(wxCommandEvent &event);
	void OnCreateNew(wxCommandEvent &event);
	wxString GetPath() const;
private:
	wxTextCtrl* dbPathInput;
};
