#include "gui/time_log/edit_parent_id.hpp"
#include <wx/treebase.h>
#include <wx/wx.h>


EditParentId::EditParentId(wxWindow* parent, const long long& MyId, Database& db)
	: wxDialog(parent, wxID_ANY, "Edit Parent ID", wxDefaultPosition, wxDefaultSize)
	, m_myId(MyId)
	, m_db(db)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* ParentIdSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* ParentId_st = new wxStaticText(this, wxID_ANY, _("Parent ID:"));
	m_ParentIdCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1)); // 入力欄をあえて広くするために、wxSize で指定

	ParentIdSizer->Add(ParentId_st, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	ParentIdSizer->Add(m_ParentIdCtrl, 1, wxEXPAND | wxRIGHT, 5);

	mainSizer->Add(ParentIdSizer, 0, wxEXPAND | wxALL, 10);


	// 下部
	m_btnSave = new wxButton(this, wxID_ANY, _("Save"));
	m_btnQuit = new wxButton(this, wxID_CANCEL, _("Quit"));

	bottomSizer->AddStretchSpacer();
	bottomSizer->Add(m_btnSave, 0);
	bottomSizer->Add(m_btnQuit, 0);
	mainSizer->Add(bottomSizer, 0, wxEXPAND | wxALL, 10);

	SetSizerAndFit(mainSizer);

	m_btnSave->Bind(wxEVT_BUTTON, &EditParentId::OnSave, this);
	m_btnQuit->Bind(wxEVT_BUTTON, &EditParentId::OnQuit, this); // 終了ボタン -> 終了イベント
}

void EditParentId::OnSave(wxCommandEvent& event) { // 保存
	
	wxString val = m_ParentIdCtrl->GetValue();

	if (val.IsEmpty()) {
		wxMessageBox(_("Parent ID is empty"), "Error", wxOK | wxICON_WARNING, this);
		return;
	}

	// 文字列を数値に変換する処理
	long long newId;
	if (!val.ToLongLong(&newId)) {
		wxMessageBox(_("Invalid ID: Please enter a number"), "Error", wxOK | wxICON_ERROR, this);
	return;
	}

	// DB 保存処理
	if (m_db.EditParentId(m_myId, newId)) {
		EndModal(wxID_OK);
	} else {
		wxMessageBox(_("Failed to update database"), "Error", wxOK | wxICON_ERROR, this);
	}
}

void EditParentId::OnQuit(wxCommandEvent& event){ // 終了
	EndModal(wxID_CANCEL);
}
