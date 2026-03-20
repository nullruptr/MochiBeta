#pragma once
#include "core/db/database.hpp"
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/wx.h>

class EditParentId : public wxDialog {
public:
	EditParentId(wxWindow* parent, const long long& MyId, Database& db);
	void OnQuit(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
private:
	wxTextCtrl* m_ParentIdCtrl;
	wxButton* m_btnSave;
	wxButton* m_btnQuit;

	Database& m_db;
	long long m_myId;
	long long m_newParentId;
};
