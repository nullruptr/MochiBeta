#include <wx/wx.h>
#include <wx/aui/aui.h>
#include "mainwnd.hpp"

Mainwnd::Mainwnd(wxWindow* parent) : wxFrame(parent, wxID_ANY, _("wxAUI Test"),
	wxDefaultPosition, wxSize(800,600),
	wxDEFAULT_FRAME_STYLE) {
        // notify wxAUI which frame to use
        m_mgr.SetManagedWindow(this);

	// テキストコントロールの作成
	wxTextCtrl* text1 = new wxTextCtrl(this, wxID_ANY, _("Pane 1"), 
			       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxTextCtrl* text2 = new wxTextCtrl(this, wxID_ANY, _("Pane 2"), 
			       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxTextCtrl* text3 = new wxTextCtrl(this, wxID_ANY, _("Main Content"), 
                                       wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

        // add the panes to the manager
	m_mgr.AddPane(text1, wxAuiPaneInfo().Left().Caption(_("Pane Number One")));
	m_mgr.AddPane(text2, wxAuiPaneInfo().Bottom().Caption(_("Pane Number Two")));
	m_mgr.AddPane(text3, wxAuiPaneInfo().CenterPane());

        // tell the manager to "commit" all the changes just made
        m_mgr.Update();
    }


Mainwnd::~Mainwnd(){
        // deinitialize the frame manager
        m_mgr.UnInit();
}
