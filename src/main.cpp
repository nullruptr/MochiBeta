#include <wx/msgdlg.h>
#include <wx/wx.h>
#include "gui/mainwnd/mainwnd.hpp"

// enum は，mainwnd.hpp に移植

class MBApp : public wxApp {
public:
	bool OnInit() override;
private:
	wxLocale locale;
};

wxIMPLEMENT_APP(MBApp);

// エントリ
bool MBApp::OnInit() {
	locale.Init(wxLANGUAGE_ENGLISH); // 使用言語設定
	locale.AddCatalogLookupPathPrefix("locale");
	locale.AddCatalog("mb");
	Mainwnd *mainwnd = new Mainwnd(nullptr); 
	mainwnd->SetTitle("MochiBeta"); // Build habits the sticky way
	mainwnd->Show(true);
	return true;
}

