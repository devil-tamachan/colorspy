#pragma once

#include "IntBar.h"

class CIntBarCtrl :
  public CDialogImpl<CIntBarCtrl>, public CWinDataExchange<CIntBarCtrl>, public CDialogResize<CIntBarCtrl>
{
public:
    enum { IDD = IDD_INTBARCTRL };

    BOOL PreTranslateMessage(MSG* pMsg){
        return CWindow::IsDialogMessage(pMsg);
    }

    CStatic m_label;
    int m_val; //[0-255]
    CIntBar m_bar;
    
    BEGIN_DDX_MAP(CIntBarCtrl)
      DDX_CONTROL_HANDLE(IDS_INTBAR_LABEL, m_label)
      DDX_CONTROL(IDS_INTBAR_BAR1, m_bar)
      DDX_UINT_RANGE(IDE_INTBAR_EDIT, m_val, 0, 255)
    END_DDX_MAP()

    BEGIN_DLGRESIZE_MAP(CIntBarCtrl)
        DLGRESIZE_CONTROL(IDS_INTBAR_BAR1, DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDE_INTBAR_EDIT, DLSZ_MOVE_X)
    END_DLGRESIZE_MAP()

    BEGIN_MSG_MAP(CIntBarCtrl)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CDialogResize<CIntBarCtrl>)
    END_MSG_MAP()

private:

    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
    {
      DoDataExchange(FALSE);
      DlgResize_Init(false, false, 0/*WS_THICKFRAME | WS_MAXIMIZEBOX | WS_CLIPCHILDREN*/);

      LONG_PTR style = GetWindowLongPtr(GWL_STYLE);
      style &= ~(WS_SIZEBOX|WS_THICKFRAME|DS_3DLOOK|WS_MINIMIZEBOX);
      SetWindowLongPtr(GWL_STYLE, style);

      return TRUE;
    }

public:
    void SetLabel(LPCTSTR label)
    {
      m_label.SetWindowText(label);
    }

    void SetIntValue(BYTE v)
    {
      m_val = v;
      m_bar.SetIntValue(v);
      DoDataExchange(FALSE);
    }

  CIntBarCtrl(void): m_val(0)
  {
  }

  ~CIntBarCtrl(void)
  {
  }
};

