#pragma once

#include "IntBarCtrl.h"

class CIntBarCtrlSet3 :
  public CWindowImpl<CIntBarCtrlSet3>
{
public:
    DECLARE_WND_CLASS(_T("IntBarCtrlSet3"));

    BEGIN_MSG_MAP_EX(CIntBarCtrlSet3)
      //MSG_WM_ERASEBKGND(OnEraseBkgnd)
      //MSG_WM_PAINT(OnPaint)
      MSG_WM_SIZE(OnSize)
    END_MSG_MAP()

    CIntBarCtrl m_intbar1;
    CIntBarCtrl m_intbar2;
    CIntBarCtrl m_intbar3;
    
    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }
    void OnSize(UINT nType, CSize size)
    {
      //m_bar1.ResizeClient(size.cx, 14);
      m_intbar1.SetWindowPos(NULL, 0, 0, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
      m_intbar2.SetWindowPos(NULL, 0, 24, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
      m_intbar3.SetWindowPos(NULL, 0, 48, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
    }

  void Init()
  {
    CRect rect;
    GetClientRect(rect);
    int w = rect.Width();
    m_intbar1.Create(m_hWnd);
    m_intbar1.SetLabel(_T("H:"));
    m_intbar1.SetWindowPos(NULL, 0, 0, w, 23, SWP_NOMOVE|SWP_NOZORDER);
    m_intbar2.Create(m_hWnd);
    m_intbar2.SetLabel(_T("S:"));
    m_intbar2.SetWindowPos(NULL, 0, 24, w, 23, SWP_NOZORDER);
    m_intbar3.Create(m_hWnd);
    m_intbar3.SetLabel(_T("V:"));
    m_intbar3.SetWindowPos(NULL, 0, 48, w, 23, SWP_NOZORDER);
  }

  void SetHSV(double *hsv)
  {
    int h = hsv[0]*255.0;
    m_intbar1.SetIntValue(h);
    int s = hsv[1]*255.0;
    m_intbar2.SetIntValue(s);
    int v = hsv[2]*255.0;
    m_intbar3.SetIntValue(v);
  }

  CIntBarCtrlSet3(void)
  {
  }

  ~CIntBarCtrlSet3(void)
  {
  }
};

