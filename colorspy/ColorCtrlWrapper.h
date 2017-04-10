#pragma once

#include "ColorCtrl.h"

class CColorCtrlWrapper :
  public CWindowImpl<CColorCtrlWrapper>
{
public:
    DECLARE_WND_CLASS(_T("ColorCtrlWrapper"));

    BEGIN_MSG_MAP(CColorCtrlWrapper)
      //MSG_WM_ERASEBKGND(OnEraseBkgnd)
		  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
      MSG_WM_SIZE(OnSize)
    END_MSG_MAP()

    CColorCtrl m_colorctrl;

	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    return (LRESULT)GetStockObject(NULL_BRUSH);
  }

    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }
    
    void OnSize(UINT nType, CSize size)
    {
      m_colorctrl.SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE|SWP_NOZORDER);
  }

    void Init()
    {
      CRect rect;
      GetClientRect(rect);
      int w = rect.Width();
      m_colorctrl.Create(m_hWnd);
      m_colorctrl.SetWindowPos(NULL, 0, 0, w, rect.Height(), SWP_NOMOVE|SWP_NOZORDER);

      LONG_PTR style = m_colorctrl.GetWindowLongPtr(GWL_STYLE);
      style |= WS_CLIPCHILDREN;
      m_colorctrl.SetWindowLongPtr(GWL_STYLE, style);
    }

public:
  void SetPickId(int id)
  {
    m_colorctrl.SetPickId(id);
  }
  void SetPickEnable(bool bEnable)
  {
    m_colorctrl.SetPickEnable(bEnable);
  }
  COLORREF GetRGB()
  {
    return m_colorctrl.GetRGB();
  }
  void SetRGB(COLORREF c)
  {
    return m_colorctrl.SetRGB(c);
  }
  CColorCtrlWrapper(void)
  {
  }

  ~CColorCtrlWrapper(void)
  {
  }
};

