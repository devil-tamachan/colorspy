#pragma once

#include "HSVView.h"
#include "PickerButton.h"
#include "ColorView.h"
#include "IntBar.h"
#include "IntBarCtrlSet3.h"

class CColorCtrl :
  public CDialogImpl<CColorCtrl>, public CWinDataExchange<CColorCtrl>, public CDialogResize<CColorCtrl>
{
public:
    enum { IDD = IDD_COLORCTRL };

    BOOL PreTranslateMessage(MSG* pMsg){
        return CWindow::IsDialogMessage(pMsg);
    }

    CIntBarCtrlSet3 m_intbarCtrlSet1;
    CHSVView m_hsvview1;
    CPickerButton m_btn_pick1;
    CColorView m_col1;
    COLORREF m_color;

    BEGIN_DDX_MAP(CColorCtrl)
      DDX_CONTROL(IDC_STATIC1, m_hsvview1)
      DDX_CONTROL(IDS_INTBARSET1, m_intbarCtrlSet1)
      DDX_CONTROL(IDS_PICK1, m_btn_pick1)
      DDX_CONTROL(IDS_COLOR1, m_col1)
    END_DDX_MAP()

    BEGIN_DLGRESIZE_MAP(CColorCtrl)
      DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_X|DLSZ_SIZE_Y/*|DLSZ_REPAINT*/)
      DLGRESIZE_CONTROL(IDS_INTBARSET1, DLSZ_SIZE_X|DLSZ_MOVE_Y/*|DLSZ_REPAINT*/)
      DLGRESIZE_CONTROL(IDS_PICK1, DLSZ_MOVE_Y)
      DLGRESIZE_CONTROL(IDS_COLOR1, DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_MSG_MAP(CColorCtrl)
		  MESSAGE_HANDLER(WM_CHANGERGB, OnChangeRGB)
      //MSG_WM_ERASEBKGND(OnEraseBkgnd)
      MSG_WM_INITDIALOG(OnInitDialog)
      CHAIN_MSG_MAP(CDialogResize<CColorCtrl>)
    END_MSG_MAP()

private:

  BOOL OnEraseBkgnd(CDCHandle /*dc*/)
  {
    return 1;
  }

  LRESULT OnChangeRGB(UINT msg, WPARAM wp, LPARAM lp, BOOL& bHandled)
  {
    m_color = (COLORREF)wp;
    ChangeRGB();

    //bHandled = FALSE;
    ::SendMessage(::GetParent(GetParent()), msg, wp, lp);

    return 1;
  }

  void ChangeRGB()
  {
    m_hsvview1.SetRGB(m_color);
    m_col1.SetColor(m_color);
    m_intbarCtrlSet1.SetCOLORREF(m_color);
    DoDataExchange(FALSE);
  }

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
  {
    DoDataExchange(FALSE);
    m_intbarCtrlSet1.Init();
    m_btn_pick1.SetPickId(0);
    m_btn_pick1.SetWindowPos(NULL, 0, 0, 16, 16, SWP_NOMOVE|SWP_NOZORDER);

    DlgResize_Init(false, false, 0);

    LONG_PTR style = GetWindowLongPtr(GWL_STYLE);
    style &= ~(WS_SIZEBOX|WS_THICKFRAME|DS_3DLOOK|WS_MINIMIZEBOX);
    SetWindowLongPtr(GWL_STYLE, style);


    return TRUE;
  }

public:
  void SetPickId(int id)
  {
    m_btn_pick1.SetPickId(id);
  }
  void SetPickEnable(bool bEnable)
  {
    m_btn_pick1.ShowWindow(bEnable?SW_SHOW:SW_HIDE);
  }
  COLORREF GetRGB()
  {
    return m_color;
  }
  void SetRGB(COLORREF c)
  {
    m_color = c;
    ChangeRGB();
  }
  void SetColorSpace(int modeColorSpace)
  {
    m_intbarCtrlSet1.SetColorSpace(modeColorSpace);
  }
  void SetDiffMode(int diffMode)
  {
    m_intbarCtrlSet1.SetDiffMode(diffMode);
  }
  void SetDiffCOLORREF(COLORREF c)
  {
    m_intbarCtrlSet1.SetDiffCOLORREF(c);
  }

  CColorCtrl(void):m_color(0)
  {
  }

  ~CColorCtrl(void)
  {
  }
};

