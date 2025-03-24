#include "nwpwin.h"
#include "res.h"

class edit : public vsite::nwp::window
{
protected:
    std::string class_name() override { return "EDIT"; }
};

class button : public vsite::nwp::window
{
protected:
    std::string class_name() override { return "BUTTON"; }
};

class list_box : public vsite::nwp::window
{
protected:
    std::string class_name() override { return "LISTBOX"; }
};

class main_window : public vsite::nwp::window
{
private:
    edit edit_control;
    button add_button;
    button remove_button;
    list_box list_control;

protected:
    int on_create(CREATESTRUCT* pcs) override;
    void on_command(int id) override;
    void on_destroy() override;
};

int main_window::on_create(CREATESTRUCT* pcs)
{
    list_control.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        0, IDC_LB, 50, 50, 150, 150);

    edit_control.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        0, IDC_EDIT, 220, 50, 150, 25);

    add_button.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        "Add", IDC_ADD, 220, 90, 150, 30);

    remove_button.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
        "Remove", IDC_REMOVE, 220, 140, 150, 30);

    return 0;
}

void main_window::on_command(int id) {
    switch (id) {
    case ID_FILE_EXIT:
        ::DestroyWindow(*this);
        break;
    case ID_HELP_ABOUT:
        ::MessageBox(*this, "Listbox Example - NWP Assignment 2", "About", MB_OK | MB_ICONINFORMATION);
        break;
    case IDC_ADD:
    {
        char buffer[256] = { 0 };
        ::GetDlgItemText(*this, IDC_EDIT, buffer, sizeof(buffer));

        if (buffer[0] != '\0') {
            ::SendDlgItemMessage(*this, IDC_LB, LB_ADDSTRING, 0, (LPARAM)buffer);
            ::SetDlgItemText(*this, IDC_EDIT, "");
            ::EnableWindow(remove_button, TRUE);
        }
    }
    break;
    case IDC_REMOVE:
    {
        int sel = ::SendDlgItemMessage(*this, IDC_LB, LB_GETCURSEL, 0, 0);

        if (sel != LB_ERR) {
            ::SendDlgItemMessage(*this, IDC_LB, LB_DELETESTRING, sel, 0);

            int count = ::SendDlgItemMessage(*this, IDC_LB, LB_GETCOUNT, 0, 0);
            if (count == 0) {
                ::EnableWindow(remove_button, FALSE);
            }
        }
    }
    break;
    }
}

void main_window::on_destroy() {
    ::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    main_window w;
    w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 2", (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_V2)));
    vsite::nwp::set_icons(instance, w, IDI_V2);
    vsite::nwp::application app;
    return app.run();
}