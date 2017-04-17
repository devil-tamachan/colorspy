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
    //int m_val; //[0,255] or [-255,+255]
    CIntBar m_bar;
    WTL::CString m_edit;
    
    BEGIN_DDX_MAP(CIntBarCtrl)
      DDX_CONTROL_HANDLE(IDS_INTBAR_LABEL, m_label)
      DDX_CONTROL(IDS_INTBAR_BAR1, m_bar)
      //DDX_UINT_RANGE(IDE_INTBAR_EDIT, m_val, 0, 255)
      DDX_TEXT(IDE_INTBAR_EDIT, m_edit)
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

    void SetIntValue(int v)
    {
      //m_val = v;
      m_edit.Format(_T("%d"), v);
      double d = v;
      d/=255.0;
      m_bar.SetDoubleValue(d);
      DoDataExchange(FALSE);
    }
    void SetDoubleValue(double v, double max = 255.0)
    {
      //m_val = v;
      int i=v*max+0.5;
      m_edit.Format(_T("%d"), i);
      m_bar.SetDoubleValue(v);
      DoDataExchange(FALSE);
    }

    void SetIntValueDiff(int v, int diff)
    {
      //m_val = v;
      double d = v;
      d/=255.0;
      d = clampDouble(d);
      m_edit.Format(_T("%+d"), diff);
      m_bar.SetDoubleValue(d);
      DoDataExchange(FALSE);
    }

    void SetDoubleValueDiff(double v, int diff)
    {
      //m_val = v;
      m_edit.Format(_T("%+d"), diff);
      m_bar.SetDoubleValue(v);
      DoDataExchange(FALSE);
    }

    /*void SetFloatValue(int v, float f)
    {
      //m_val = v;
      m_edit.Format(_T("%f"), f);
      m_bar.SetIntValue(v);
      DoDataExchange(FALSE);
    }*/

    void SetStrValue(double v, ATL::CString str)
    {
      //m_val = v;
      m_edit = str;
      m_bar.SetDoubleValue(v);
      DoDataExchange(FALSE);
    }

  CIntBarCtrl(void)//: m_val(0)
  {
    m_edit = _T("0");
  }

  ~CIntBarCtrl(void)
  {
  }
};

