#include <wx/wx.h>
#include <wx/aui/aui.h>

class Mainwnd : public wxFrame {
public:
Mainwnd(wxWindow* parent);
virtual ~Mainwnd();
private:
    wxAuiManager m_mgr;
};
