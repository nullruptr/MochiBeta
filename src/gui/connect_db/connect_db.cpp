#include "gui/connect_db/connect_db.hpp"
#include "core/db/database.hpp"
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/wx.h>

ConnectDB::ConnectDB(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "Connection Dialog", wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* pathSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* label = new wxStaticText(this, wxID_ANY, _("Database Path:"));
	dbPathInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1)); // 入力欄をあえて広くするために、wxSize で指定
	wxButton* btnBrowse = new wxButton(this, wxID_ANY, _("..."));

	pathSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	pathSizer->Add(dbPathInput, 1, wxEXPAND | wxRIGHT, 5);
	pathSizer->Add(btnBrowse, 0);

	mainSizer->Add(pathSizer, 0, wxEXPAND | wxALL, 10);

	// 下部
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL); // 下部ボタン用サイザ

	wxButton* btnConnect = new wxButton(this, wxID_ANY, _("Connect"));
	wxButton* btnQuit = new wxButton(this, wxID_CANCEL, _("Quit"));

	bottomSizer->AddStretchSpacer();
	bottomSizer->Add(btnConnect, 0, wxRIGHT, 5);
	bottomSizer->Add(btnQuit, 0);

	mainSizer->Add(bottomSizer, 0, wxEXPAND | wxALL, 10);
	SetSizerAndFit(mainSizer);

	btnBrowse->Bind(wxEVT_BUTTON, &ConnectDB::OnBrowse, this);
	btnConnect->Bind(wxEVT_BUTTON, &ConnectDB::OnConnect, this);
}

void ConnectDB::OnBrowse(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(
		this, // 親指定
		_("Select SQLite DB"), // タイトルバーに表示される文字列
		"", // 初期ディレクトリ
		"", // 初期選択ファイル名
		"SQLite DB (*.db)|*.db|All files (*.*)|*.*", // 表示名|拡張子|表示名|拡張子
		wxFD_OPEN | wxFD_FILE_MUST_EXIST // 開くダイアログ | 存在しないファイルは選択できない
	);

	// ファイルダイアログをモーダル表示し、OKが返されたとき、これを実行
	if (openFileDialog.ShowModal() == wxID_OK) 
	{
		dbPathInput->SetValue(openFileDialog.GetPath()); // TextCtrl にこれを戻す
	}
}

void ConnectDB::OnConnect(wxCommandEvent &event){

	wxString wxPath = dbPathInput->GetValue(); // TextCtrlから要素抽出
	std::string path = wxPath.ToStdString(); // wxString を、std::string へ変換
	
	if (wxPath.IsEmpty()){ // 空文字判定
		wxMessageBox(_("Path is empty"));
		return;
	}

	if (!wxFileExists(wxPath)){ // パスの存在判定
		wxMessageBox(_("File does not exist"));
		return;
	}

	EndModal(wxID_OK); // 正当なファイルパスなら通過
}

wxString ConnectDB::GetPath() const {
	return dbPathInput->GetValue();
}
